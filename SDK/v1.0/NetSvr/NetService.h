/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxNetService.h
 * @brief:xxxNetService 
 * @author:xxxNetServicenbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-05-31
 */

#pragma once 



#include "NetClient.h"
#include "UserObject.h"
#include "PtrCycle.h"

#include <set> 
#include <vector>
#include <map>
#include <mutex> 
#include <condition_variable>



class CNetService 
{
public:
	CNetService();
	virtual ~CNetService();
public:
	virtual bool	OnInitialUpdate();
	
	virtual bool	OnTimeOut(struct tm* pTime);

	
	virtual void	OnTerminate();
public:
	virtual bool	OnNetConnect(CNetClient* pNet);						//网络连接

	virtual bool	OnNetMsg(CNetClient* pNetClient, PHEADER pHeader);	//网络消息达到

	virtual bool	OnNetBreak(CNetClient* pNetClient);					//网络断开

	virtual bool	OnNetTickCnt(CNetClient* pNetClient);				//网络定时调用
public:
	void	ActiveEpollThread();

	void	ActiveWorkThread();
public:
	
public:
	int			m_nPort;
protected:
	CPtrCycle<CNetClient>			m_listWorkClient;	
	std::mutex						m_clsWork;
	std::condition_variable			m_condWork;
private:
	std::set<CNetIO*>				m_listNetIO;
	std::map<int, CNetIO*>			m_mapNetIO;
private:
	bool		m_bStop;
};
extern class CNetService* g_pNetService;
