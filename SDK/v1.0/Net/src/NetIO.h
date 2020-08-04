/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		NetIO.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-09
 */

#pragma once 

#include "Socket.h"
#include "Protocol.h"

#include <sys/epoll.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/types.h> 
#include <atomic>
#include <vector>
#define EPOLL_EVENT_TYPE_CLOSE		0x80000000

typedef std::vector<char>	Buffer;
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
	virtual void PermitRead();
	virtual void PermitWrite();
	virtual void ForbidRead();
	virtual void ForbidWrite();

	virtual bool IsReadable();
	virtual bool IsWriteable();
public:
	virtual bool OnSend();
	
	virtual bool OnRecv();

	virtual bool OnClose();

	virtual void OnTickCnt();							//定时调用
public:
	virtual bool OnMsg(PHEADER& pHeader);			//消息到达接口
public:
	virtual void ProcessMsg();

	void SetOwner(CNetIO* pOwner);
	CNetIO* GetOwner() {return m_pOwner;};
protected:
	CNetIO*					m_pOwner;
public:
	unsigned int			m_nEventType;
	unsigned int			m_nNewEventType;
private:
	std::atomic_int			m_nRef;	
};
