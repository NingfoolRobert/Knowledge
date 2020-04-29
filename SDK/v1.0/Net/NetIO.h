#pragma once 


#include "Socket.h"
#include <sys/epoll.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <atomic>
#define EPOLL_EVENT_TYPE_CLOSE		0x80000000

class CNetIO: 
	public CSocket
{
public:
	CNetIO();
	virtual ~CNetIO();
public:
	int		AddRef();
	void	Release();
public:
	
	virtual bool OnSend();
	
	virtual bool OnRecv();

	virtual bool OnClose();

	virtual bool OnTerminate();
public:
	unsigned int			m_nEventType;
	unsigned int			m_nNewEventType;
private:
	std::atomic_int			m_nRef;	
};
