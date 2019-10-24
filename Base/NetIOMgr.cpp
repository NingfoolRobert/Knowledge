#include "NetIOMgr.h"
	

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

int CNetIOMgr::AcceptIO(strcut epoll_event &ev)
{
	return 0;
}
	

void StartListen()
{
	return ;
}

void CNetIOMgr::StartEpoll()
{
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
