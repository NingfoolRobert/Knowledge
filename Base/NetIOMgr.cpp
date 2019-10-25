#include "NetIOMgr.h"
#include "NetClient.h"


bool CNetIOMgr::OnInitialUpdate()
{

	m_fdEP = epoll_create(10000);
	if(0 == m_fdEP)
	{
		LogError("%s(%d) Create epoll handle fail.", __FILE__, __LINE__);
		return false;
	}
	//
	if(!m_SockListen.Create())
	{
		LogError("Create Socket fail. Port:%d", m_nPort);
		return false;
	}
	//bind 
	if(!m_SockListen.Bind(m_nPort))	
	{
		LogError("Bind Socket fail. Port: %d", m_nPort);
		return false;
	}
	//
	m_SockListen.SetBlockMode();
	//
	if(!m_SockListen.Listen())
	{
		LogError("Listen Socket fail. Port: %d", m_nPort);
		return false;
	}

	struct epoll_event ev;
	ev.data.fd = m_SockListen.Detach();
	ev.events  = EPOLLIN | EPOLLET;
	
	epoll_ctl(m_fdEP, EPOLL_CTL_ADD, m_SockListen.Detach(), &ev);
}


int CNetIOMgr::AcceptIO(struct epoll_event &ev)
{
	while(true)
	{
		
		int confd = m_SockListen.Accept();
		if(confd < 0)
		{
			if(errno ==  EAGAIN)
			{
				// 接收socket 完成 
				break;
			}
			else if(errno == ECONNRESET)
			{

			}
			else if(errno == EINTR)
			{
				continue;
			}
			else 
			{
				LogError("Accept Socket occure error: %d", errno);
				return 0;
			}
		}
		//
		CNetClient* pNetClient = CreateNetClient();
		if(nullptr == pNetClient)
		{
			LogError("%s(%d) Create NetClient Error.", __FILE__, __LINE__);
			return 0;
		}
		pNetClient->Attach(confd);
		pNetClient->SetBlockMode();
		
		//TODO  Add pNetClient && epoll confd ;
	}
	return 0;
}

void StartListen()
{
	return ;
}

void CNetIOMgr::StartEpoll()
{
	
	UpdateEvent();
	
	m_listFd.clear();

	auto nSize = m_listClient.size();
	m_listFd.resize(nSize + 1);

	int nfdCount = epoll_wait(m_fdEP, &m_listEvent[0], nSize + 1, -1);
	for_each(m_listEvent.begin(), m_listEvent.end(),[this](struct epoll_event& ev){
			if(ev.data.fd == m_SockListen.Detach())	//监听到有新连接 
			{
				AcceptIO(ev);
			}
			else if(ev.events & EPOLLIN)		//有新消息到来 
			{
					
			}
			else if(ev.events & EPOLLOUT)		//可向客户端发送消息 
			{

			}

			});

}

void CNetIOMgr::LoadEvent(EventMap::value_type& event_pair)
{
	CNetClient* pClient = event_pair.first;
	int oldEvent  = event_pair.second;
	int newEvent = pClient->m_nState;

	int fd = pClient->m_Socket.Detach();
	

	struct epoll_event ev;
	if(newEvent & EPOLLCLOSE)

	{
		if(oldEvent)
			epoll_ctl(m_fdEP, EPOLL_CTL_DEL, fd, &ev);
		m_listClient[fd]  = nullptr;
		delete pClient;
	}
	else 
	{
		int Status = 0;
		if(newEvent & EPOLLOUT) 
			Status |= EPOLLOUT;
		if(newEvent & EPOLLIN)
			Status |= EPOLLIN;
		if(Status)
		{
			ev.events = Status;
			ev.data.ptr = pClient;
			epoll_ctl(m_fdEP, (oldEvent & (POLLIN | POLLOUT))? EPOLL_CTL_MOD: EPOLL_CTL_DEL, fd, &ev);
		}
		else if(oldEvent)
			epoll_ctl(m_fdEP, EPOLL_CTL_DEL, fd,  &ev);
	}
	
}


void CNetIOMgr::UpdateEvent()
{
	
}

void CNetIOMgr::TriggerEvent(const struct epoll_event& ev)
{
	int fd = ev.data.fd;
	//监听套接字 
	if(fd == m_SockListen.Detach())
	{
		//thread  io 
		AcceptIO(ev);
		return ;
	}
	//
	auto it  = m_listfd.find(fd);
	if(it == m_listfd.end())
	{
		//TODO delete the fd 
		return ;
	}
	
	auto pNetClient = it->second;
	
	if(ev.events & EPOLLIN)
	{
		pNetClient->RecvMsg();
	}
	else if(ev.events & EPOLLOUT)
	{
		pNetClient->SendMsg();
	}
	else if (ev.events & (EPOLLERR | EPOLLHUP))
	{

	}
}
