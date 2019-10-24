/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		NetIOMgr.h
 * @brief:		netio manager 
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-24
 */

#pragma once 

#include "Socket.h"
#include "Log.h"

#include <sys/epoll.h>
#include <map>
#include <vector>
#include <set>

class CNetClient;

class CNetIOMgr 
{
public:
	CNetIOMgr();
	virtual ~CNetIOMgr();

public:
	virtual bool OnInitialUpdate();
	
	int AcceptIO(struct epoll_event& ev);
	
	void StartListen();
public:
	int	GetListenFD(){return m_SockListen.Detach();}
	
	void StartEpoll();
public:
	int			m_nPort;


protected:
	CSocket		m_SockListen;
	
	int			m_fdEP;						// epoll 文件描述符
private:
	std::unordered_map<int, CNetClient*>  m_listClient;			//key:fd, value:CNetClient 

	std::unordered_map<CNetClient*, int>	m_listfd;			//Key:CNetClient ,value:fd 

	std::set<CNetClient*>			m_listClient;

//	std::vector<int>				m_listFd;					//存放epoll监听到数据接口	
	std::vector<struct epoll_event> m_listEvent;				//epoll 监听到的事件 
};
