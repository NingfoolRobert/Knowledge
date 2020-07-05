#include "NetService.h"
#include "AutoLock.h"
#include "LogFile.h"

#include <sys/epoll.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <thread> 
#include <algorithm> 
#include <memory>


CNetService::CNetService(void):m_pAcceptIO(nullptr),m_nTimeOut(100),m_bStop(false)
{

}

CNetService::~CNetService()
{

}

bool CNetService::OnInitualUpdate()
{
	m_pAcceptIO = new CAcceptIO;
	if(nullptr == m_pAcceptIO)
	{
		LogError("Init Net AcceptIO fail.");
		return false;
	}

	if(!m_pAcceptIO->InitialUpdate(m_nPort, this))
	{
		return false;
	}
	
	m_ep = epoll_create(256);
	if(m_ep <= 0)
	{
		LogError("Create epoll fd  fail.");
		return false;
	}
	int fd = m_pAcceptIO->Detach();
	struct epoll_event evAccept;
	evAccept.events = EPOLLIN | EPOLLET;
	evAccept.data.fd = fd; 
	evAccept.data.ptr = m_pAcceptIO;
	
	epoll_ctl(m_ep, EPOLL_CTL_ADD, fd, &evAccept);


	m_mapNetIO.insert(std::make_pair(m_pAcceptIO->Detach(), m_pAcceptIO));

	std::thread tr1(&CNetService::ActiveEpollThread, this);
	tr1.detach();
	
	for(int i = 0; i < WORKFUNC_THREAD_COUNT; ++i)
	{
		std::thread tr(&CNetService::ActiveWorkFuncThread, this);
		tr.detach();
	}

	LogInfo("Net Service Listen Success IP:Port = LocalIP:%d.", m_nPort);
    return true;
}
    
bool CNetService::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}

	LogInfo("Connection Current Information: %d/%d/%d.", m_mapNetIO.size() - 1 , m_listModIO.size(), m_listDelClientS.size());
	return true;
}

bool CNetService::OnSecondeIdle()
{
	CAutoLock locker(&m_clsEpollLock);
	CNetClient* pNetClient = nullptr;
	for(auto it = m_listClient.begin(); it != m_listClient.end(); ++it)
	{
		pNetClient =*it;
		if(pNetClient)
		{
			pNetClient->OnSecondIdle();
		}
	}
	return true;
}

void CNetService::OnTerminate()
{
	m_bStop = true;
	CNetClient* pNetClient = nullptr;
	CAutoLock locker(&m_clsEpollLock);
	
	for(auto it = m_listClient.begin(); it != m_listClient.end(); ++it)
	{
		pNetClient = *it;
		if(nullptr == pNetClient)
			continue;
		pNetClient->Terminate();		
	}
	
}
	
CNetClient*	CNetService::OnCreateNetClient()
{
	return new CNetClient;
}

bool CNetService::OnNetConnect(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
		return false; 
    //
    if(!AddClient(pNetClient))
    {
        LogError("Add Client fail. IP:Port=%d:%d", pNetClient->m_dwIP, pNetClient->m_nPort);
    }
	pNetClient->OnConnect();
	return true;
}

void CNetService::OnNetConnect(int& fd)     //网络链接上来
{
    CNetClient* pNetClient = new CNetClient;
    if(nullptr == pNetClient)
    {
        LogError("memmory error.");
        return ;
    }
    //
    if(!pNetClient->Init(fd, this))
    {
        LogError("NetClient Init fail.fileno:%d", fd);
        return ;
    }
    //
    if(!AddClient(pNetClient))
    {
        LogError("Add Client fail. IP:Port=%d:%d", pNetClient->m_dwIP, pNetClient->m_nPort);
    }
}

bool CNetService::OnNetMsg(CNetClient* pNetClient, PHEADER pHeader) //网络消息到达
{
    if(nullptr == pNetClient || nullptr == pHeader)
    {
        LogError("init CNetClient is nullptr or PHEADER is nullptr.");
		return false;
    }	


	auto pUser = pNetClient->GetUserObject();
	ADD_REF(pUser);
	pUser->OnMsg(pHeader);
	DEC_REF(pUser);
	return true;
}


void CNetService::ActiveWorkFuncThread()
{
    CNetClient* pNetClient = nullptr;
    while(true)
    {
        std::unique_lock<std::mutex>    locker(m_clsWorkLock);
        m_condWork.wait(locker, [&]()->bool{return !m_listWorkClient.empty();});
        locker.unlock();
        m_clsWorkLock.lock();
        if(!m_listWorkClient.empty())
        {
			pNetClient = m_listWorkClient.front();
            m_listWorkClient.pop();
        }
        m_clsWorkLock.unlock();
        //
		if(pNetClient)
			pNetClient->ActiveRecvThread();

//		m_clsWorkLock.lock();
//		if(!m_listDelClientS.empty())
//		{
//			pNetClient = m_listDelClientS.front();
//			m_listDelClientS.pop();
//		}
//		m_clsWorkLock.unlock();
//		
//		if(pNetClient)
//		{
//			delete pNetClient;
//			pNetClient = nullptr;
//		}
    }
}
    

void CNetService::ActiveEpollThread()
{
	LogInfo("Start Net Listen IP:Port = LocalIP:%d", m_nPort);
    while(!m_bStop)
    {
        UpdateEvent();
        FdSockList    EventList;
        int nMax = m_mapNetIO.size();
        EventList.resize(nMax);
        int nCount = epoll_wait(m_ep, &EventList[0], nMax, m_nTimeOut);
        //
		if(nCount > 0)
			for_each(EventList.begin(), EventList.begin() + nCount, std::bind(&CNetService::TriggerEvent, this, std::placeholders::_1));
		else if (nCount < 0)
		{
			if(errno != EINTR)
				LogError("epoll wait fail.errno: %d", errno);
		}
    }
}

void CNetService::TriggerEvent(const struct epoll_event& ev)
{
    CNetIO* pNetIO  = (CNetIO*) ev.data.ptr;
	//	
	if(ev.events & (EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLPRI))
		pNetIO->OnRecv();
	if(ev.events & EPOLLOUT)
		pNetIO->OnSend();
}

void CNetService::UpdateEvent()
{
	FdNetIOMap _list;
	{
		CAutoLock locker(&m_clsEpollLock);
		for(auto it = m_listModIO.begin(); it != m_listModIO.end();++it)
		{
			_list.insert(std::make_pair(it->first, it->second));
			m_mapNetIO.insert(std::make_pair(it->first, it->second));
		}
		m_listModIO.clear();
	}
	
	for_each(_list.begin(), _list.end(), std::bind(&CNetService::LoadEvent, this, std::placeholders::_1));

}
    
bool CNetService::AddClient(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
	{
		return false;
	}
	
	CAutoLock locker(&m_clsEpollLock);
	m_listModIO.insert(std::make_pair(pNetClient->Detach(), pNetClient));		
	pNetClient->AddRef();	
	return true;
}

bool CNetService::DelClient(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
	{
		return true;
	}
		
	std::unique_lock<std::mutex> locker(m_clsWorkLock);
	m_listDelClientS.push(pNetClient);
	m_condWork.notify_one();
	return true;
}
	
void CNetService::OnNetBreak(CNetClient* pNetClient)				//网络断开
{
	if(nullptr == pNetClient)
	{
		return ;
	}
	//
	{
		CAutoLock locker(&m_clsEpollLock);
		m_listModIO.insert(std::make_pair(pNetClient->Detach(), pNetClient));		
		
		auto it = m_listClient.find(pNetClient);
		if(it != m_listClient.end())
		{
			m_listClient.erase(it);
		}
	}
}
	
void CNetService::LoadEvent(std::unordered_map<int, CNetIO*>::value_type &value)
{
	int fd = value.first;
	CNetIO* pNetIO = value.second;
	if(nullptr == pNetIO)
	{
		return ;
	}
	
	unsigned int nStatus = pNetIO->m_nNewEventType;
	pNetIO->m_nNewEventType = 0;

	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = pNetIO->m_nEventType | nStatus;
	ev.data.ptr = pNetIO;
	//删除
	if(nStatus == EPOLL_EVENT_TYPE_CLOSE)
	{
		epoll_ctl(m_ep, EPOLL_CTL_DEL, fd, &ev);
		
		auto it = m_mapNetIO.find(fd);
		if(it != m_mapNetIO.end())
		{
			m_mapNetIO.erase(it);
		}
			
		pNetIO->m_nEventType = nStatus;
		pNetIO->OnClose();
		pNetIO->Release();
		return ;
	}
	//	
	if(nStatus)
	{
		if(nStatus &(EPOLLIN | EPOLLOUT))
		{
			LogInfo("Add Client");
		}
		epoll_ctl(m_ep, nStatus & (EPOLLIN | EPOLLOUT)? EPOLL_CTL_ADD : EPOLL_CTL_MOD, fd, &ev);
		pNetIO->m_nEventType |= nStatus;
	}
}
