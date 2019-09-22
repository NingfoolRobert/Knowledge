#include "epoll.h"


CEpoll::CEpoll()
{
	m_ep = epoll_create(0xfff);
}

CEpoll::~CEpoll()
{

}

bool  CEpoll::OnInitialUpdate(CNetService* pService, int cnThread)
{

	if(pService == nullptr)
		return false;
	m_pService = pService;

	m_ThreadPoll.init(cnThread);
	return true;
}

bool  CEpoll::AddClient(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
		return false;

	//TODO
	return true;
}

void CEpoll::LoadEvent()
{

}

void CEpoll::UpdateEvent()
{

}

void CEpoll::UpdateEventMap()
{

}


void CEpoll::TriggerEvent()
{
	CSocket* pSock = (CSocket*)m_event.data.ptr;
	if(m_event.events &(EPOLLIN | EPOLLERR | EPOLLHUP))
		pSock->OnSend();
	if()

void CEpoll::Poll(int timeout)
{

}
bool CEpoll::Add(CSocket* pSocket)
{
	return false;
}
bool CEpoll::Del(int fd)
{
	return false;
}

void CEpoll::Poll(int nTimeout)
{
	int nSize = 100;
	int nCount = epoll_wait(m_ep, &m_listFD[0], nSize, nTimeout);
	
	for(int i = 0; i < nCount; ++i)
	{



	}
	
	
}


