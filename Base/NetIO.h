/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		NetIO.h
 * @brief:		NetIO
 * @author:		nbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2019-12-15
 */

#pragma once
#include "Socket.h"

#define EPOLLCLOSE	0x80000000

class CNetIO: public CSocket
{
public:
	CNetIO();
	virtual ~CNetIO();

public:
//	virtual bool OnInitialUpdate();

	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual bool OnTerminate();
public:

	virtual void OnSend(){}
	
	virtual void OnRecv() {}

	virtual void OnClose() {}
public:
	virtual int Send(const char* pBuf, int nLength);
	
	virtual int Recv(char* pBuf, int nLength);

	virtual int Recv(char* pBuf, int nLength, bool& bRecvAll);
	
//	virtual bool Close();

public:
	void UpdateEventType(int nType);

public:
	int			m_nEvent;					//当前NetIO状态信息	
	int			m_nNewEvent;				//新的IO状态信息，EPOLLIN, EPOLLOUT, EPOLLCLOSE;
	
public:
	unsigned int	m_dwIP;
	int				m_nPort;
protected:
	
};

