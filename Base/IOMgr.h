/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		IOMgr.h
 * @brief:		管理网络IO
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-25
 */

#pragma once 

#include <unordered_map>
#include <unordered_set>
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <set>

#include "Protocol.h"
#include "Log.h"
#include "NetIO.h"
#include "AcceptIO.h"
#include "AutoLock.h"


struct HashFunc
{
	std::size_t operator()(const CNetIO* pClient) const 
	{
		using std::size_t;
		using std::hash;
		return (hash<int>()(pClient->m_dwIP) ^ (hash<int>()(pClient->m_nPort) << 1));	
	}
};
struct Equal
{
	bool operator()(const CNetIO* lhs, const CNetIO* rhs)const 
	{
		return (lhs->m_dwIP == rhs->m_dwIP) && (lhs->m_nPort && rhs->m_nPort);
	}
};

struct cmpSet{

	bool operator()(const CNetIO* lc, const CNetIO* rc)
	{
		if(lc->m_dwIP == rc->m_dwIP)
			return lc->m_nPort < rc->m_nPort;
		return lc->m_dwIP < rc->m_dwIP;
	}
};

typedef std::unordered_map<CNetIO*, int, HashFunc, Equal> EventMap;
typedef std::unordered_set<CNetIO*, HashFunc, Equal> EventSet;
typedef std::unordered_map<int, CNetIO*> IOMap;

class CNetService;

class CIOMgr
{
public:
	CIOMgr();
	virtual ~CIOMgr();
public:
	virtual bool OnInitialUpdate(CNetService* pNetService,CAcceptIO* pAcceptIO);

	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();
	
	virtual bool OnTerminate();

public:
	bool AddNetIO(CNetIO* pNetIO);
public:

	void LoadEvent(const EventMap::value_type& event_pair);

	void UpdateEvent();

	void TriggerEvent(const struct epoll_event& ev);
	//thread tr1  
	void	EpollThread(int nTimeOut = 1000);
protected:
	bool AddListenNetIO(CNetIO* pNetIO);	
protected:
	int					m_ep;					//epoll 文件句柄

	CNetService*		m_pNetService;
	
	CAcceptIO*			m_pAcceptIO;
protected:
	IOMap		m_mapIO;			//fd->pNetClient
	EventMap	m_mapEvent;				//pNetClient -> Status;	
	EventSet	m_setEvent;				//Status change NetClient;
	
	std::vector<struct epoll_event>		m_listEvent;
	CObjectLock m_clsLock;
	IOMap		m_mapNew;				//新增IO 
};
