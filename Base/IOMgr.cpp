
#include "IOMgr.h"
	




void CIOMgr::LoadEvent(const EventMap::value_type & event_pair)
{
	CNetClient* pNetClient = event_pair.first;

	int	oldEvent = event_pair.second;
	int newEvent = pNetClient->m_nEvent;
	int fd = pNetClient->Detach();
	
	struct epoll_event ev;
	if(newEvent & EPOLLCLOSE)
	{
		if(oldEvent)
			epoll_ctl(m_ep, EPOLL_CTL_DEL,fd,&ev);
		m_mapClient[fd] = nullptr;
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

	EventMap map;
	for(auto it = m_mapNew.begin(); it != m_mapNew.end(); ++it)
	{
		CNetClient* pNetClient = (*it).second;
		m_mapClient[(*it).first] = pNetClient;
		if(pNetClient->m_nNewEvent)
			m_setEvent.insert(pNetClient);
	}
	m_mapNew.clear();
	for(auto it = m_setEvent.begin(); it != m_setEvent.end(); ++it)
	{
		map.insert(std::make_pair(*it, (*it)->m_nStatus));
		(*it)->m_nEvent		= (*it)->m_nNewEvent;
		(*it)->m_nNewEvent = 0;
	}
	m_setEvent.clear();
	
	for_each(map.begin(),map.end(), std::ptr_fun(LoadEvent));
}


void CIOMgr::TriggerEvent(const struct epoll_event & ev)
{
	CNetClient* pNetClient  = (CNetClient*) ev.data.ptr;
	if(ev.events & (EPOLLIN | EPOLLERR | EPOLLHUP))
		pNetClient->RecvMsg();
	if(ev.events & EPOLLOUT)
		pNetClient->SendMsg();
}
	//thread tr1  

void	EpollThread()
{

}
