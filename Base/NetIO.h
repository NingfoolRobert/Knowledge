

#pragma once
#include "Socket.h"

class CNetIO: public CSocket
{
public:
	CNetIO();
	virtual ~CNetIO();
public:

	virtual void OnSend(){}
	
	virtual void OnRecv() {}
public:
	virtual int Send(const char* pBuf, int nLength);
	
	virtual int Recv(char* pBuf, int nLength);

	virtual int Recv(char* pBuf, int nLength, bool& bRecvAll = false);

public:
	void UpdateEventType(int nType);

public:
	int			m_nEvent;					//当前NetIO状态信息	
	int			m_nNewEvent;				//新的IO状态信息，EPOLLIN, EPOLLOUT, EPOLLCLOSE;
	
protected:
	
};

