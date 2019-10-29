#include "SThreadPool.h"


bool CSThreadPool::Init(int nIdleCount, int nMaxThreadCount, int nTimeOut)
{
	m_nMaxCount		= nMaxThreadCount;
	m_nDefaultCount = nIdleCount;
	m_nTimeOut		= nTimeOut;
	//
	for(int i = 0; i < nIdleCount; ++i)
	{
		std::thread tr1(std::mem_fn(&CSThreadPool::Run), this);
		tr1.detach();
		m_nCurCount++;
	}
	return true;
}

bool CSThreadPool::AddTask(Task& task)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	m_listTask.push(task);
	if(m_nIdelCount > 0)
	{
		//通知线程 
		m_condLock.notify_one();
	}
	else if(m_nCurCount < m_nMaxCount)
	{
		std::thread tr1(std::mem_fn(&CSThreadPool::Run), this);
		m_nCurCount++;
		tr1.detach();
	}
	
	return true;
}

bool CSThreadPool::Terminate()
{
	if(m_bStop)
	{
		return true;
	}

	std::unique_lock<std::mutex> locker(m_clsLock);
	m_bStop = true;
	if(m_nCurCount)	
	{
		//唤醒所有等待的线程;
		if(m_nIdelCount)
		{
			//唤醒 
			m_condLock.notify_all();
		}
		//等待任务执行完毕
		while(m_nCurCount)
		{
			//等待 
			m_condLock.wait(locker);
		}
	}

	return true;
}


void CSThreadPool::Run()
{
	bool bTimeout = false;
	while(true)
	{
		bTimeout = false;
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_nIdelCount++;	
		//
		while(m_listTask.empty() && !m_bStop)
		{
			std::cv_status  nState = m_condLock.wait_for(locker, std::chrono::seconds(m_nTimeOut));
			if(nState == std::cv_status::timeout && m_nIdelCount > m_nDefaultCount)
			{
				bTimeout = true;
				break;
			}
		}
		m_nIdelCount--;
		//消费任务
		if(!m_listTask.empty())
		{	
			Task task = m_listTask.front();
			m_listTask.pop();
			task();
		}
		//退出线程池 并销毁当前线程
		if(m_bStop && m_listTask.empty())
		{
			m_nCurCount--;
			break;
		}
		//超时线程销毁 
		if(bTimeout)
		{
			m_nCurCount--;
			break;
		}
	}
}

