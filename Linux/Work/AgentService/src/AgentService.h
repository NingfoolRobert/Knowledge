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


#include <future>


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

	virtual bool InvokeTerminate();

	virtual bool OnTerminate();

public:

	bool SendLogMsg(const std::string strDirName, std::vector<std::string>& listLogItem);
public:
	bool PostMsg(const char* pszTopic, CBuffer* pBufMsg);

	bool PostTask(Task& task);

	template<class F, class... Args>
	auto PostTask(F&&f, Args&&... args) 
	->std::future<typename std::result_of<F(Args...)>::type>
	{
		return m_pSThreadPool->AddTask(std::forward<F>(f), std::forward<Args>(args)...);
	//	using return_type = typename std::result_of<F(Args...)>::type;

	//	auto task = std::make_shared< std::packaged_task<return_type()> >(
	//			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
	//			); 
	//	std::future<return_type> res = task->get_future();
	//	
	//	m_pSThreadPool->AddTask([task](){(*task)();});

	//	return res;
	}

public:
	CBuffer*  GetBuffer(int nLength, const char* pszFileName = "", int nLine =  0);
	
	bool ReclaimBuffer(CBuffer* pBuffer);

	unsigned long  GetPubSerialNum(int nIncrement = 1);
protected:
//	bool GetXMLProfileString(const char* pszSection, const char* pszKey, const char* pszDefaultValue, char* pszValue, const char* pszFileName = nullptr);
bool GetXMLAttributeString(const char* pszSection, const char* pszKey, const char* pszDefaultValue, char* pszValue, const char* pszFileName/* = nullptr*/);

private:

	CSThreadPool*						m_pSThreadPool;
	CZMQPub*							m_pPubLog;
	std::shared_ptr<CLogMgr>			m_pLogMgr;
};
extern class CAgentService* g_ciccAgentService;
