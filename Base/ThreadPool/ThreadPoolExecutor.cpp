#include "ThreadPoolExecutor.h"

CThreadPoolExecutor::CWorker::CWorker(CThreadPoolExecutor * pThreadPool, Runnable * pFirstTask) : 
m_pThreadPool(pThreadPool),
m_pFirstTask(pFirstTask),
m_bRun(true)
{
	
}

CThreadPoolExecutor::CWorker::~CWorker()
{
}

/**
  ִ������Ĺ����̡߳�
  ��ǰû������ʱ��
  �����ǰ�߳�����������С�߳������������̣߳�
  ����ִ��������򣬽��߳�����ͷŵ�
**/
void CThreadPoolExecutor::CWorker::Run()
{
	Runnable * pTask = NULL;
	while(m_bRun)
	{
		if(NULL == m_pFirstTask)
		{
			pTask = m_pThreadPool->GetTask();
		}
		else
		{
			pTask = m_pFirstTask;
			m_pFirstTask = NULL;
		}

		if(NULL == pTask)
		{
			CAutoLock locker(&m_pThreadPool->m_clsThreadPoolLock);
			if(m_pThreadPool->GetThreadPoolSize() > m_pThreadPool->m_minThreads)
			{
				ThreadPoolItr itr = m_pThreadPool->m_ThreadPool.find(this);
				if(itr != m_pThreadPool->m_ThreadPool.end())
				{
					m_pThreadPool->m_ThreadPool.erase(itr);
					m_pThreadPool->m_TrashThread.insert(this);
				}
				m_bRun = false;
			}
			else
			{
				ThreadPoolItr itr = m_pThreadPool->m_TrashThread.begin();
				while(itr != m_pThreadPool->m_TrashThread.end())
				{
					(*itr)->Join();
					delete (*itr);
					m_pThreadPool->m_TrashThread.erase(itr);
					itr = m_pThreadPool->m_TrashThread.begin();
				}
			}
			continue;
		}
		else
		{
			pTask->Run();
			pTask = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

CThreadPoolExecutor::CThreadPoolExecutor(void) : 
m_bRun(false),
m_bEnableInsertTask(false)
{
}

CThreadPoolExecutor::~CThreadPoolExecutor(void)
{
	Terminate();
}

bool CThreadPoolExecutor::Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTasks)
{
	CAutoLock locker(&m_clsThreadPoolLock);
	if(minThreads == 0)
	{
		return false;
	}
	if(maxThreads < minThreads)
	{
		return false;
	}
	m_minThreads = minThreads;
	m_maxThreads = maxThreads;
	m_maxPendingTasks = maxPendingTasks;
	unsigned int i = m_ThreadPool.size();
	for (; i < minThreads; i++)
	{
		//�����߳�
		CWorker * pWorker = new CWorker(this);
		if (NULL == pWorker)
		{
			return false;
		}
		m_ThreadPool.insert(pWorker);
		if (!pWorker->Start())
		{
			// TO DO Output Log
			;
		}
	}
	m_bRun = true;
	m_bEnableInsertTask = true;
	return true;
}

bool CThreadPoolExecutor::Execute(Runnable * pRunnable)
{
	CAutoLock locker(&m_clsThreadPoolLock);
	if(!m_bEnableInsertTask)
	{
		return false;
	}
	if(NULL == pRunnable)
	{
		return false;
	}
	if(m_Tasks.size() >= m_maxPendingTasks)
	{
		if(m_ThreadPool.size() < m_maxThreads)
		{
			CWorker * pWorker = new CWorker(this, pRunnable);
			if(NULL == pWorker)
			{
				return false;
			}
			m_ThreadPool.insert(pWorker);
			pWorker->Start();
		}
		else
		{
			return false;
		}
	}
	else
	{
		CAutoLock lockTask(&m_clsTaskLock);
		m_Tasks.push_back(pRunnable);
	}
	return true;
}

Runnable * CThreadPoolExecutor::GetTask()
{
	CAutoLock locker(&m_clsTaskLock);
	Runnable * Task = NULL;
	if(!m_Tasks.empty())
	{
		Task = m_Tasks.front();
		m_Tasks.pop_front();
	}
	return Task;
}

unsigned int CThreadPoolExecutor::GetThreadPoolSize()
{
	return m_ThreadPool.size();
}

void CThreadPoolExecutor::Terminate()
{
	CAutoLock locker(&m_clsThreadPoolLock);;
	m_bEnableInsertTask = false;
	while(m_Tasks.size() > 0)
	{
		Sleep(1);
	}
	m_bRun = false;
	m_minThreads = 0;
	m_maxThreads = 0;
	m_maxPendingTasks = 0;
	while(m_ThreadPool.size() > 0)
	{
		Sleep(1);
	}
	ThreadPoolItr itr = m_TrashThread.begin();
	while(itr != m_TrashThread.end())
	{
		(*itr)->Join(0);
		delete (*itr);
		m_TrashThread.erase(itr);
		itr = m_TrashThread.begin();
	}
}
