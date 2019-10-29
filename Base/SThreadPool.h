/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		SThreadPool.h
 * @brief:		策略线程池 
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-29
 */

#include <vector>
#include <queue>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <map>


typedef std::function<void(void)> Task;


class CSThreadPool
{
public:
	CSThreadPool();
	virtual ~CSThreadPool();

public:
	class CPolicy
	{
		public:
			CPolicy();
			~CPolicy();
		public:
			int			m_nCountCurrent;
			int			m_nMaxThreadCount;
			
	};
public:
	bool  Init(int nIdleCount = 5, int nMaxThreadCount = 50, int nTimeOut = 60);

	bool  AddTask(Task& task);

	bool Terminate();
public:
	void Run();
private:
	int							m_nTimeOut;						//超时时间(秒)
	bool						m_bStop;
	int							m_nIdelCount;
	int							m_nCurCount;
	int							m_nDefaultCount;
	int							m_nMaxCount;
	CPolicy						m_cPolicy;						//策略管理
	std::queue<Task>			m_listTask;						//任务队列 

	std::mutex					m_clsLock;
	std::condition_variable     m_condLock;

private:
	CSThreadPool(const CSThreadPool&){}
	const CSThreadPool& operator = (const CSThreadPool&){return CSThreadPool();}
	


};
