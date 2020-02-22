#pragma once 


#include "Socket.h"

#define EPOLL_EVENT_TYPE_CLOSE		0x80000000

class CNetIO: 
	public CSocket
{
public:
	CNetIO();
	virtual ~CNetIO();
public:
	
	virtual bool OnSend();
	
	virtual bool OnRecv();

	virtual bool OnClose();
public:
	unsigned int			m_nEventType;
	unsigned int			m_nNewEventType;
};
