/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		StreamIO.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-09
 */

#pragma once 
#include "NetIO.h"

#include <vector> 
#include <mutex> 
#include <list>


//typedef std::vector<char> Buffer;

//typedef bool (*PMSGFUNC)(PHEADER);

class CStreamIO :
	public CNetIO
{
public:
	CStreamIO();
	virtual ~CStreamIO();
public:
	bool SendMsg(Buffer* pBuf);
	
	bool SendMsg(PHEADER pMsg);
public:
//	virtual bool  OnMsg(PHEADER pMsg) = 0;
public:
	virtual bool OnSend();
	
	virtual bool OnRecv();

	virtual bool OnClose();
public:
	virtual void PermitRead();
	virtual void PermitWrite();
	virtual void ForbidRead();
	virtual void ForbidWrite();

	virtual bool IsReadable();
	virtual bool IsWriteable();
public:
	virtual void ProcessMsg();
public:
	unsigned int	m_dwIP;
	int				m_nPort;
public:
	time_t			m_tLast;
	time_t			m_tBreak;
private:
	std::mutex					m_clsRecvLock;
	std::list<Buffer*>			m_listRecvBuf;	
	Buffer*						m_pRecvBuf;
	unsigned int				m_dwRecvLen;

	std::mutex					m_clsSendLock;
	std::list<Buffer*>			m_listSendBuf;
	Buffer*						m_pSendBuf;
	unsigned int				m_dwSendLen;
};



