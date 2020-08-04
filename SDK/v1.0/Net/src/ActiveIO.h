/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		ActiveIO.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-16
 */
#pragma once 
#include "NetIO.h"
#include <mutex>


class CActiveIO:
	public CNetIO 
{
public:
	explicit CActiveIO();
	virtual ~CActiveIO();
public:
	virtual bool OnInit(CNetIO* pNetIO);
	
	virtual void OnTickCnt(){}				//定时调用
	
	virtual void OnBreak();					//断开调用
	
	virtual bool OnMsg(PHEADER& pHeader);	//消息到达接口
public:

	void Terminate();						//主动断开
public:
	virtual bool OnRecv();

	virtual bool OnSend();
	
	virtual bool OnClose();
public:
	virtual void PermitRead();
	
	virtual bool IsReadable();
	
	virtual void ForbidWrite();
public:
	bool SendMsg(PHEADER pHeader);				//Sync Send 

	bool PeerMsg(Buffer* pBuf);
public:
	unsigned int		m_dwHostIP;
	int					m_nPort;
public:
	time_t				m_tBreak;				//断开时间
	time_t				m_tLastestMsg;			//最新消息时间
private:
	std::mutex			m_clsSendLock;
};
