#pragma once

#include <czmq.h>


class CNet
{
public:
	CNet();
	virtual ~CNet();

public:
	void*	Create(int nType);
	bool	Bind (const char* pszHostAddr, int nPort = 0); //IP 
	bool	Connect(const char* pszIP, int nPort = 0); //IP:nPort 
	bool	Send(const char* pszBuffer, int nLength); 
	bool	Recv(char* pszBuffer, int nLength);
	
	
protected:
	int			m_nType;		//zmq 类型
	int			m_nPort;		//端口号
	char		m_szIP[16];		//IP 
	void*		m_sock;
	zctx_t*		m_ctx;
};


