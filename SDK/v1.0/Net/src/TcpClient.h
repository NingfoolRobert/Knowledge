/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		TcpClient.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-17
 */
#pragma once 
#include "ActiveIO.h"


class CTcpClient:
	public CActiveIO 
{
public:
	CTcpClient();
	virtual ~CTcpClient();
public:
	virtual bool  OnMsg(PHEADER& pMsg);

	virtual void  OnBreak();
	
	virtual void  OnTickCnt();
public:
	bool	Init(const char*  pszHostIP, int nPort, bool bAutoIOMgr = false);

	bool	Init(unsigned int dwHostIP, int nPort, bool bAutoIOMgr = false);

	bool	Reconnect();
public:
	bool	SendSyncMsg(PHEADER pMsg, Buffer* pBuf); 
public:
	bool	m_bAutoIOMgr;				//添加到IO管理器
};
