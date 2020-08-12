/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		PassiveIO.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-16
 */

#pragma once 


#include "NetIO.h"
#include <mutex>

class CPassiveIO:
	public CNetIO
{
public:
	CPassiveIO();
	virtual ~CPassiveIO();
public:
	virtual bool OnRecv();
	virtual bool OnClose();

	virtual void OnTickCnt();
public:
	virtual void OnBreak();
public:
	virtual bool OnMsg(PHEADER& pMsg);
public:
	bool SendMsg(PHEADER pMsg);

	void Terminate();
public:
	unsigned int	m_dwIP;
	int				m_nPort;
public:
	time_t			m_tLastMsg;
	time_t			m_tBreak;
private:
	std::mutex		m_clsSendLock;
};

