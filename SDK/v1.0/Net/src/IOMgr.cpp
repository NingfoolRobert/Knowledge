#include "IOMgr.h"
#include "LogFile.h"
#include <iostream>
#include <thread> 
#include <functional> 
#include <algorithm> 
#include <future> 
#include <memory> 
#include <unistd.h> 
#include <signal.h> 

void handle_pipe(int sig)
{

}

class CIOMgr*		g_pIOMgr = nullptr;

CIOMgr::CIOMgr():m_nTimeOut(10), m_cnWorkNetThread(1), m_bStop(false)
{
	g_pIOMgr = this;
}
	
CIOMgr::~CIOMgr()
{

}

bool CIOMgr::OnInitialUpdate()
{
//	signal(SIGPIPE,SIG_IGN);
	
	struct sigaction sa;
	sa.sa_handler = handle_pipe;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);

	m_epFD = epoll_create(1024);
	if(m_epFD < 0)
	{
		LogError("create epoll fd fail.");
		return  false;
	}
	for(auto i = 0; i < m_cnWorkNetThread; ++i)
	{
		std::thread tr(&CIOMgr::ActiveWorkThread, this);
		tr.detach();
	}
	std::thread tr1(&CIOMgr::ActivePollThread, this);
	tr1.detach();
	return true;
}
	
bool CIOMgr::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
		return false;

	LogInfo("NetIOMgr Cnt: %d, MsgCnt:%d", m_mapNetIO.size(),  m_listWorkNetIO.size());
	return true;
}
	
void CIOMgr::OnInvokeTerminate()
{
	m_clsLock.lock();
	for(auto it = m_mapNetIO.begin(); it != m_mapNetIO.end(); ++it)
	{
		auto pNetIO = it->second;
		pNetIO->OnClose();
	}
	m_clsLock.unlock();
}

bool CIOMgr::OnSecondIdle()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	for(auto it = m_mapNetIO.begin(); it != m_mapNetIO.end(); ++it)
	{
		auto pNetIO = it->second;
		if(nullptr == pNetIO)	 continue;
		pNetIO->OnTickCnt();
	}
	return true;
}


void CIOMgr::OnTerminate()
{
	m_clsLock.lock();
	m_bStop = true;
	m_condWork.notify_all();
}

	
bool CIOMgr::AddNetIO(CNetIO* pNetIO)
{
	if(nullptr == pNetIO)
		return false;
	pNetIO->AddRef();
	pNetIO->SetBlockMode();
	std::unique_lock<std::mutex> locker(m_clsLock);
	auto it = m_mapNetIO.find(pNetIO->Detach());
	if(it == m_mapNetIO.end())
	{
		m_mapNetIO.insert(std::make_pair(pNetIO->Detach(),  pNetIO));
	}

	m_cnWaitNet++;

	return true;
}

	
void CIOMgr::ActiveWorkThread()
{
	CNetIO* pNetIO = nullptr;
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsWorkLock);
		m_condWork.wait(locker, [&]()->bool{return m_listWorkNetIO.size() || m_bStop;});
		locker.unlock();
		while(m_listWorkNetIO.size())
		{
			m_clsWorkLock.lock();
			if(m_listWorkNetIO.size())
			{	
				pNetIO = m_listWorkNetIO.front();
				m_listWorkNetIO.pop_front();
			}	
			m_clsWorkLock.unlock();
			if(nullptr == pNetIO)
				continue;
			
			pNetIO->ProcessMsg();
			pNetIO->Release();
		}
	}
	
}
	
	
void CIOMgr::ActivePollThread()
{
	int cnMaxEvent = 0;

	std::vector<struct epoll_event> listEvent;
	while(!m_bStop)
	{
		UpdateEvent();
		cnMaxEvent = m_mapNetIO.size();	
		listEvent.resize(m_mapNetIO.size());
		int cnEvents = epoll_wait(m_epFD,  &listEvent[0], cnMaxEvent, m_nTimeOut);
		if(cnEvents > 0)
			for_each(listEvent.begin(), listEvent.end(), std::bind(&CIOMgr::TriggerEvent, this, std::placeholders::_1));
	}
}
	
void CIOMgr::UpdateEvent()
{
	std::vector<CNetIO*> listNetIO;
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		auto it = m_mapNetIO.begin();
		while(it != m_mapNetIO.end())
		{
			auto pNetIO = it->second;
			if(nullptr == pNetIO)
			{
				++it;
				continue;
			}
			if(pNetIO->m_nNewEventType)
			{
				listNetIO.push_back(pNetIO);	
			}
			if(pNetIO->m_nNewEventType & EPOLL_EVENT_TYPE_CLOSE)
			{
				m_mapNetIO.erase(it++);
				continue;
			}
		
			++it;
		}
	}
	for_each(listNetIO.begin(), listNetIO.end(), std::bind(&CIOMgr::LoadEvent, this, std::placeholders::_1));
}
	
void CIOMgr::TriggerEvent(const struct epoll_event& ev)
{
	CNetIO* pNetIO = (CNetIO*)ev.data.ptr;
	if(pNetIO == nullptr) return ;
	if(ev.events & (EPOLLIN | EPOLLERR | EPOLLHUP))
		pNetIO->OnRecv();
	if(ev.events & EPOLLOUT)
		pNetIO->OnSend();
}
	
	
void CIOMgr::LoadEvent(CNetIO* pNetIO)
{
	if(nullptr == pNetIO)
		return ;
	
	unsigned int dwOldEvent = pNetIO->m_nEventType;
	unsigned int dwNewEvent = pNetIO->m_nNewEventType;

	int fd = pNetIO->Detach();
	struct epoll_event ev;
	if(dwNewEvent & EPOLL_EVENT_TYPE_CLOSE)
	{
		if(dwOldEvent)
		{	
			epoll_ctl(m_epFD, EPOLL_CTL_DEL, fd, &ev);	// Del 
			CNetIO* pOwner = pNetIO->GetOwner();
			if(pOwner) pOwner->Release();
			pNetIO->Release();
		}
	}	
	else 
	{
		int nStatus = 0;
		if(dwNewEvent & EPOLLIN)
			nStatus |= EPOLLIN;
		if(dwNewEvent & EPOLLOUT)
			nStatus |= EPOLLOUT;
		if(nStatus)
		{
			ev.data.ptr = pNetIO;
			ev.events = nStatus;
			int nRet = epoll_ctl(m_epFD, (dwOldEvent & (EPOLLIN | EPOLLOUT))? EPOLL_CTL_MOD: EPOLL_CTL_ADD, fd, &ev);
			if(nRet) 
			{
				LogError("add fd fail. %d, %d", nRet, fd);
			}
			pNetIO->m_nEventType |= dwNewEvent;
			pNetIO->m_nNewEventType = 0;
		}
	}
}

bool CIOMgr::OnNetMsg(CNetIO* pNetIO)
{
	if(nullptr == pNetIO)
		return false; 

	pNetIO->AddRef();
	m_clsWorkLock.lock();
	m_listWorkNetIO.push_back(pNetIO);
	m_clsWorkLock.unlock();
	m_condWork.notify_one();
	return true;
}
	
bool CIOMgr::DelNetIO(CNetIO* pNetIO)
{
	if(nullptr == pNetIO)
		return true;
	m_clsLock.lock();
	auto it = m_mapNetIO.find(pNetIO->Detach());
	if(it != m_mapNetIO.end())
	{
		m_mapNetIO.erase(it);
	}
	m_clsLock.unlock();

	return true;
}
	
bool CIOMgr::OnNetTick(CPassiveIO* pClient)					//定时消息调用
{
	return true;
}

bool CIOMgr::OnNetMsg(CPassiveIO* pNetClient, PHEADER& pMsg)
{
	return true;
}
	
bool CIOMgr::OnNetBreak(CPassiveIO* pNetClient)				//网络断开
{
	return true;
}
	
bool CIOMgr::OnNetConnect(CPassiveIO* pNetClient)			//链接到来
{
	return true;
}
