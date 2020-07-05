/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:			NetClient.h
 * @brief:			NetClient
 * @author:			nbf,nbf0537@163.com
 * @version:		1.0
 * @date:			2020-05-31
 */

#pragma once 
#include "NetIO.h"
#include "BufferMgr.h"
#include "Protocol.h"
#include "AutoLock.h"
#include "PtrCycle.h"

class CUserObject;

class CNetClient:
	public CNetIO 
{
public:
	CNetClient();
	virtual ~CNetClient();
public:
	
public:
	virtual bool OnRecv();
	virtual bool OnSend();
	virtual bool OnClose();
public:
	bool	SendMsg(PHEADER pMsg);
public:
	void	ProcessMsg();
	
public:
	void	BindUserObject(CUserObject* pUserObject);
	bool	IsBindUserObject();
	CUserObject* GetUserObject();
public:
	unsigned int		m_dwIP;
	int					m_nPort;
public:
	time_t				m_tBreak;
private:
	CObjectLock				m_clsRecvLock;	
	CPtyCycle<CBuffer>		m_listRecvBuffer;
	unsigned int			m_dwRecvSerialNum;
	CBuffer*				m_pRecvBuf;
	unsigned int			m_dwRecvLen;
private:	
	CObjectLock				m_clsSendLock;
	CPtrCycle<CBuffer>		m_listSendBuffer;
	unsigned int			m_dwSendSerialNum;
	CBuffer*				m_pSendBuf;
	unsigned int			m_dwSendLen;
private:
	CUserObject*			m_pUserObject;
};
