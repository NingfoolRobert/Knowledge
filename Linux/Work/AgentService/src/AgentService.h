/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		AgentService.h
 * @brief:		日志采集服务
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-11-07
 */

#pragma once 
//#include "NetObjService.h"
#include "Service.h"
#include "SThreadPool.h"
#include "LogMgr.h"
#include "zmqPub.h"
#include "BufferMgr.h"

class CAgentService:
	public CService
{
public:
	CAgentService();
	virtual ~CAgentService();
public: 
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();
	
	virtual bool OnTerminate();

public:

	bool SendLogMsg(const std::string strDirName, std::vector<std::string>& listLogItem);
public:
	bool PostMsg(const char* pszTopic, CBuffer* pBufMsg);

public:
	CBuffer*  GetBuffer(int nLength, const char* pszFileName = "", int nLine =  0);
	
	bool ReclaimBuffer(CBuffer* pBuffer);

	unsigned long  GetPubSerialNum(int nIncrement = 1);
private:

	CSThreadPool*						m_pSThreadPool;
	CZMQPub<CAgentService>*				m_pPubLog;
	std::shared_ptr<CLogMgr>			m_pLogMgr;
};
extern class CAgentService* g_ciccAgentService;
