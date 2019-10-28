/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		UserObject.h
 * @brief:		处理用户消息对象
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-24
 */

#pragma once
#include "Protocol.h"


class CNetClient;

class CUserObject
{
public:
	CUserObject();
	virtual ~CUserObject();

public:
	
	virtual bool OnMsg(PHEADER pMsg) = 0;

	virtual bool OnConnect();

	virtual bool OnSecondIdle();

	virtual bool OnClose();
public:
	
public:
	bool SendMsg(PHEADER pMsg);

	bool SendZipMsg(PHEADER pMsg);

public:
	
	void SetPeerAddr(unsigned int dwIP, int nPort);

protected:
	CNetClient*		m_pNetClient;
	
	int				m_nPort;

	unsigned int	m_dwIP;	
};
