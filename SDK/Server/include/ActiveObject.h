/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxActiveObject.h
 * @brief:xxxActiveObject 
 * @author:xxxActiveObjectnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2019-12-25
 */

#pragma once 

#include <set>
#include <map>
#include <mutex>
#include <condition_variable>
#include "ThreadPoolExecutor.h"
#include "TaskDefine.h"


class CActiveObject  
{
public:
	CActiveObject(void);
	virtual ~CActiveObject(void);
public:
	bool Init(int cnMaxThread = 1, int cnThread = 1, int nPendTask = 100);
	
	void SetPendingTask(int cnPendingTask);

	void SetOwner(CActiveObject* pOwner);

	void Terminate();

	void PrintfInfo();
public:
	virtual bool OnTimer(PTIMERHEADER pTimer);
	
	virtual bool OnEvent(PEVENTHEADER pEvent);
public:
	bool PostEvent(PEVENTHEADER pEvent);

	bool PostEvent(CBuffer* pBuffer);

	bool SetTimer(PTIMERHEADER pTimer, int nSecond);

	bool SetTimerMili(PTIMERHEADER pTimer, int nMiliSecond);
public:
	void ActiveTimerThread();
private:
	CThreadPoolExecutor*		m_pThreadPool;
private:
	bool											m_bEnableTimer;
	bool											m_bStopTimer;
	CObjectLock										m_clsTimerLock;	
	std::set<CBuffer*, compTimer>					m_listTimer;	
	CThread*										m_pTimerThread;
	
private:
	CActiveObject*									m_pOwner;
};
