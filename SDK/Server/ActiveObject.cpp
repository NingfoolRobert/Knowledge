#include "ActiveObject.h"

#include <sys/select.h> 
#include <sys/time.h> 
	
class CBufferMgr*	g_BufferMgr = nullptr;
class TimerThreadTask: public Runnable 
{
public:
	TimerThreadTask(CActiveObject* pObj):m_pObject(pObj){}
	virtual ~TimerThreadTask(){}
	virtual void Run()
	{
		m_pObject->ActiveTimerThread();
	}
private:
		CActiveObject* m_pObject;
};

class TimerThreadTask* g_pTimerTask = nullptr;

CActiveObject::CActiveObject(void):m_pThreadPool(nullptr),m_bEnableTimer(false),m_bStopTimer(true), m_pTimerThread(nullptr)
{
	
}

CActiveObject::~CActiveObject(void)
{
	if(m_pThreadPool)
	{
		delete m_pThreadPool;
		m_pThreadPool = nullptr;
	}
	
	if(m_pTimerThread)
	{
		delete m_pTimerThread;
		m_pTimerThread = nullptr;
	}
}

bool CActiveObject::Init(int cnMaxThread/* = 1*/, int cnThread/* = 1*/, int nPendTask/* = 100*/)
{
	m_pThreadPool = new CThreadPoolExecutor;
	if(nullptr == m_pThreadPool)
	{
		return false;
	}
	cnMaxThread = cnMaxThread < cnThread? cnThread:cnMaxThread;
	if(!m_pThreadPool->Init(cnThread, cnMaxThread, nPendTask))
	{
		return false;
	}

	return true;
}
	
void CActiveObject::SetPendingTask(int cnPendingTask)
{
	if(nullptr == m_pThreadPool)
	{
		return ;
	}
	m_pThreadPool->SetPendingTask(cnPendingTask);	
}
//对象终止释放	
void CActiveObject::Terminate()
{
	CAutoLock locker(&m_clsTimerLock);
	m_bStopTimer = true;
	for(auto it = m_listTimer.begin(); it != m_listTimer.end(); ++it)
	{
		auto pBuf = *it;
		delete pBuf;
	}
	m_listTimer.clear();
	
	m_pThreadPool->Terminate();
}

bool CActiveObject::OnTimer(PTIMERHEADER pTimer)
{
	return false;
}

bool CActiveObject::OnEvent(PEVENTHEADER pEvent)
{
	return false;
}

bool CActiveObject::PostEvent(PEVENTHEADER pEvent)
{
	if(pEvent == nullptr)
	{
		return false;
	}

	CBuffer* pBuf = g_BufferMgr->GetBuffer(pEvent->dwLength + sizeof(EVENTHEADER));
	if(pBuf == nullptr)
	{
		//LogError
		return false;
	}

	if(!pBuf->Append(pEvent, pEvent->dwLength + sizeof(EVENTHEADER)))
	{
		//LogError
		return false;
	}
	
	Runnable* pTask = new CEvent(this, pBuf);
	if(nullptr == pTask)
	{
		//LogError 
		return false;
	}
	
	return AddTask(pTask);
	
}
	
bool CActiveObject::PostEvent(CBuffer* pBuffer)
{
	if(pBuffer == nullptr)
	{
		return false;
	}

	CBuffer* pBuf = g_BufferMgr->GetBuffer(pBuffer->GetBufLen());
	if(pBuf == nullptr)
	{
		//LogError
		return false;
	}

	pBuf->Exchange(pBuffer);
	
	Runnable* pTask = new CEvent(this, pBuf);
	if(nullptr == pTask)
	{
		//LogError 
		return false;
	}
	
	return AddTask(pTask);
	
}

bool CActiveObject::SetTimer(PTIMERHEADER pTimer, int nSecond)
{
	return SetTimerMili(pTimer, nSecond * 1000);
}

bool CActiveObject::SetTimerMili(PTIMERHEADER pTimer, int nMiliSecond)
{
	if(nullptr == pTimer)
	{
		return false;
	}
	//check TimerThread Enable
	if(!m_bEnableTimer)
	{
		CAutoLock locker(&m_clsTimerLock);
		//Init TimerThread;
		g_pTimerTask = new TimerThreadTask(this);
		if(g_pTimerTask == nullptr)
		{
			//LogError 
			return false;
		}
		m_pTimerThread = new CThread(g_pTimerTask);
		if(nullptr == m_pTimerThread)
		{
			return false;
		}
		m_bStopTimer = false;	
		m_pTimerThread->Start();
		m_bEnableTimer = true;
	}

	if(m_bStopTimer)
	{
		//LogError
		return false;
	}

	CBuffer* pBuf = g_BufferMgr->GetBuffer(pTimer->dwLength + sizeof(TIMERHEADER));
	if(pBuf == nullptr)
	{
		//LogError
		return false;
	}
	
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	long lTimerOver = tv.tv_sec * 1000 + tv.tv_usec / 1000 + nMiliSecond;
	pTimer->lTimerOver = lTimerOver; 
	if(!pBuf->Append(pTimer, pTimer->dwLength + sizeof(TIMERHEADER)))
	{
		//LogError
		return false;
	}
	
	Runnable* pTask = new CTimer(this, pBuf);
	if(nullptr == pTask)
	{
		//LogError 
		return false;
	}
	if(nMiliSecond <= 0)
	{
		return AddTask(pTask);
	}
	else 
	{
		CAutoLock locker(&m_clsTimerLock);
		m_listTimer.insert(pBuf);
	}

	return true;
}

void CActiveObject::ActiveTimerThread()
{
	//auto tNow = std::chrono::system_clock::now();//
	while(!m_bStopTimer)
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 100 * 1000;

		select(0, nullptr, nullptr, nullptr, &tv);
		
		gettimeofday(&tv, nullptr);
		unsigned long lTimerNow = tv.tv_sec * 1000 + tv.tv_usec  / 1000;
		while(true)
		{
			m_clsTimerLock.Lock();
			auto it = m_listTimer.begin();
			if(it == m_listTimer.end())
			{
				m_clsTimerLock.UnLock();
				break;
			}
			auto pBuffer = *it;
			PTIMERHEADER pTimer = (PTIMERHEADER)pBuffer->GetBufPtr();
			if(pTimer->lTimerOver <= lTimerNow)	
			{
				CBuffer* pBuf = nullptr;
				auto pTask = new CTimer(this, pBuf);
				if(pTask == nullptr)
				{
					//TODO LOG
					break;
				}
				m_listTimer.erase(it);
				AddTask(pTask);
				m_clsTimerLock.UnLock();
				continue;
			}
			
//			m_clsTimerLock.unlock();
			m_clsTimerLock.UnLock();
			break;
		}

				
	}
}
	
bool CActiveObject::AddTask(Runnable* pRunnable)
{
	if(nullptr == pRunnable)
	{
		return false;
	}
	
	return m_pThreadPool->Execute(pRunnable);
}
