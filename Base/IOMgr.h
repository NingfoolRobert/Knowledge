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
#include "NetClient.h"

struct HashFunc
{
	std::size_t operator()(const CNetClient* pClient) const 
	{
		using std::size_t;
		using std::hash;
		return (hash<int>()(pClient->m_dwIP) ^ (hash<int>()(pClient->m_nPort) << 1));	
	}
};
struct Equal
{
	bool operator()(const CNetClient* lhs, const CNetClient* rhs)const 
	{
		return (lhs->m_dwIP == rhs->m_dwIP) && (lhs->m_nPort && rhs->m_nPort);
	}
};

struct cmpSet{

	bool operator()(const CNetClient* lc, const CNetClient* rc)
	{
		if(lc->m_dwIP == rc->m_dwIP)
			return lc->m_nPort < rc->m_nPort;
		return lc->m_dwIP < rc->m_dwIP;
	}
};

typedef std::unordered_map<CNetClient*, int, HashFunc, Equal> EventMap;
typedef std::set<CNetClient*, cmpSet> EventSet;
typedef std::unordered_map<int, CNetClient*> IOMap;


class CIOMgr
{
public:
	CIOMgr();
	virtual ~CIOMgr();
public:

	void LoadEvent(const EventMap::value_type& event_pair);

	void UpdateEvent();

	void TriggerEvent(const struct epoll_event& ev);
	//thread tr1  
	void	EpollThread();
protected:
	int			m_ep;					//epoll 文件句柄
protected:
	IOMap		m_mapClient;			//fd->pNetClient
	EventMap	m_mapEvent;				//pNetClient -> Status;	
	EventSet	m_setEvent;				//Status change NetClient;
	IOMap		m_mapNew;				//新增IO 
};
