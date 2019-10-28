
#include "IOMgr.h"
#include <functional>
#include <algorithm>
#include <thread>
#include <future> 


CIOMgr::CIOMgr()
{
	m_pNetService = nullptr;
	m_pAcceptIO = nullptr;
	
}

CIOMgr::~CIOMgr()
{

}

bool CIOMgr::OnInitialUpdate(CNetService* pNetService,CAcceptIO* pAcceptIO)
{
	if(nullptr == pNetService || nullptr == pAcceptIO)
	{
		return false;
	}
	
	m_pNetService = pNetService;
	m_pAcceptIO = pAcceptIO;

	AddListenNetIO(pAcceptIO);	

	//TODO 启动监听端口 
	
	std::thread tr1(std::mem_fn(&CIOMgr::EpollThread), this, 1000);
	tr1.detach();	
	return true;
}

bool CIOMgr::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}
	return true;
}

bool CIOMgr::OnSecondIdle()
{
	return true;
}

bool CIOMgr::OnTerminate()
{
	return true;
}




void CIOMgr::LoadEvent(const EventMap::value_type & event_pair)
{
	CNetIO* pNetClient = event_pair.first;

	int	oldEvent = event_pair.second;
	int newEvent = pNetClient->m_nEvent;
	int fd = pNetClient->Detach();
	
	struct epoll_event ev;
	if(newEvent & EPOLLCLOSE)
	{
		if(oldEvent)
			epoll_ctl(m_ep, EPOLL_CTL_DEL,fd,&ev);
		m_mapIO[fd] = nullptr;
		delete pNetClient;
	}
	else 
	{
		int status = 0;
		if(newEvent & EPOLLIN)
			status |= EPOLLIN;
		if(newEvent & EPOLLOUT)
			status |= EPOLLOUT;
		if(status)
		{
			ev.events = status;
			ev.data.ptr = pNetClient;
			epoll_ctl(m_ep, (oldEvent & (EPOLLOUT | EPOLLIN))? EPOLL_CTL_MOD: EPOLL_CTL_ADD, fd, &ev);
		}
		else if(oldEvent)
		{
			epoll_ctl(m_ep, EPOLL_CTL_DEL, fd, &ev);	
		}
	}
}


void CIOMgr::UpdateEvent()
{

	EventMap mapEvent;
	for(auto it = m_mapNew.begin(); it != m_mapNew.end(); ++it)
	{
		CNetIO* pNetClient = (*it).second;
		m_mapIO[(*it).first] = pNetClient;
		if(pNetClient->m_nNewEvent)
			m_setEvent.insert(pNetClient);
	}
	m_mapNew.clear();
	for(auto it = m_setEvent.begin(); it != m_setEvent.end(); ++it)
	{
		mapEvent.insert(std::make_pair(*it, (*it)->m_nEvent));
		(*it)->m_nEvent		= (*it)->m_nNewEvent;
		(*it)->m_nNewEvent = 0;
	}
	m_setEvent.clear();
	
	for_each(mapEvent.begin(),mapEvent.end(), bind(&CIOMgr::LoadEvent,this, std::placeholders::_1));
}


void CIOMgr::TriggerEvent(const struct epoll_event& ev)
{
	CNetIO* pNetClient  = (CNetIO*) ev.data.ptr;
	if(ev.events & (EPOLLIN | EPOLLERR | EPOLLHUP))
		pNetClient->OnRecv();
	if(ev.events & EPOLLOUT)
		pNetClient->OnSend();
	
	pNetClient->OnClose();
}
	//thread tr1  

void CIOMgr::EpollThread(int nTimeOut/* = 1000*/)
{
	while(true)
	{
		//TODO 循环调用
		UpdateEvent();
	
		int nMaxEvents = m_mapIO.size();
		m_listEvent.clear();
		m_listEvent.resize(nMaxEvents);
		
		int nEvents = epoll_wait(m_ep, &m_listEvent[0], nMaxEvents, nTimeOut);
	
		if(nEvents > 0)
			for_each(m_listEvent.begin(), m_listEvent.end(), bind(&CIOMgr::TriggerEvent, this, std::placeholders::_1));
	}
}

bool CIOMgr::AddNetIO(CNetIO* pNetIO)
{
	if(nullptr == pNetIO)
		return false;
	CAutoLock locker(&m_clsLock);
	m_mapNew.insert(std::make_pair(pNetIO->Detach(),pNetIO));
//	m_setEvent.insert(pNetIO);
	return true;
}

bool CIOMgr::AddListenNetIO(CNetIO* pNetIO)
{
	if(nullptr == pNetIO)
	{
		return false;
	}
	m_mapIO.insert(std::make_pair(pNetIO->Detach(), pNetIO));

	struct epoll_event ev;
	ev.data.fd = pNetIO->Detach();
	ev.events = pNetIO->m_nEvent;
	ev.data.ptr = pNetIO;
	epoll_ctl(m_ep, EPOLL_CTL_ADD, ev.data.fd, &ev);
	return true;
}
