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

#include "ThreadPoolExecutor.h"
#include <set>
#include <map>
#include <mutex>
#include <condition_variable>

#include "TaskDefine.h"


class CActiveObject  
{
public:
	CActiveObject(void);
	virtual ~CActiveObject(void);
public:
public:
	
	virtual bool OnTimer(PTIMERHEADER pTimer);
	
	virtual bool OnEvent(PEVENTHEADER pEvent);
public:
	
	bool PostEvent(PEVENTHEADER pEvent);
	
	bool SetTimer(PTIMERHEADER pTimer, int nSecond);

	bool SetTimerMili(PTIMERHEADER pTimer, int nMiliSecond);
public:
	void ActiveTimerThread(CThread* pThread);
private:
	bool AddTask(Runnable* pRunnable);
private:
	CThreadPoolExecutor*		m_pThreadPool;
private:
	bool						m_bEnableTimer;
	std::mutex					m_clsTimerLock;
	std::condition_variable		m_condTimer;	
	std::set<PTIMERHEADER>		m_listTimer;	
	CThread						m_TimerThread;
};
extern class CActiveObject* g_ActiveObject;
