/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		IOMgr.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-09
 */

#pragma once 

#include "LogFile.h"
#include "StreamIO.h"
#include "PassiveIO.h"
#include "AcceptIO.h"
#include "GFunc.h"

#include <map>
#include <unordered_map>
#include <vector> 
#include <list>
#include <mutex> 
#include <condition_variable> 

typedef std::unordered_map<int, CNetIO*>	MAPEvent;
class CIOMgr 
{
public:
	CIOMgr();
	virtual ~CIOMgr();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnSecondIdle();
	
	virtual void OnTerminate();
	
	virtual void OnInvokeTerminate();
public:
	
public:
	bool	AddNetIO(CNetIO* pNetIO);

	bool	DelNetIO(CNetIO* pNetIO);

	bool	OnNetMsg(CNetIO* pNetIO);								//消息达到
public:
	virtual bool	OnNetTick(CPassiveIO* pClient);					//定时消息调用

	virtual bool	OnNetMsg(CPassiveIO* pNetClient, PHEADER& pMsg); //消息到达接口
public:
	void	ActiveWorkThread();
	
	void	ActivePollThread();

public:
	void	UpdateEvent();

	void	TriggerEvent(const struct epoll_event& ev);

	void	LoadEvent(CNetIO* pNetIO);
public:
	unsigned int		m_nTimeOut;
	int					m_cnWorkThread;
private:
	bool				m_bStop;
private:
	int					m_epFD;
private:
	std::mutex								m_clsLock;
	std::vector<struct epoll_event>			m_listEvent;
	std::unordered_map<int, CNetIO*>		m_mapNetIO;
private:
	std::mutex								m_clsWorkLock;
	std::condition_variable					m_condWork;
	std::list<CNetIO*>						m_listWorkNetIO;
};

extern class CIOMgr*		g_pIOMgr;
