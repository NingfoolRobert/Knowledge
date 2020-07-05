/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxUserObject.h
 * @brief:xxxUserObject 
 * @author:xxxUserObjectnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-04-21
 */

#pragma once 

#include "Protocol.h"
#include "Available.h"
#include <time.h> 

class CNetClient;

class CUserObject: 
	public CAvailable 
{
public:
	CUserObject();
	virtual ~CUserObject();
public:
	virtual bool	OnMsg(PHEADER pMsg);

	virtual void	OnTickCnt();

	virtual bool	OnBreak();

	virtual bool	OnConnect();
public:

	void	BindNetClient(CNetClient* pNetClient);

	bool	IsBindNetClient();
	
	bool	SendMsg(PHEADER pMsg);

	bool	SendMsg(PHEADER pMsg, bool bAsyncSend = true);

	bool	SendMsg(CBuffer* pBuffer, bool bAsyncSend = true);

private:
	
public:
	time_t				m_tBreak;
	time_t				m_tConnect;
public:
	unsigned int		m_dwIP;
	int					m_nPort;
private:
	CNetClient*			m_pNetClient;	
};
