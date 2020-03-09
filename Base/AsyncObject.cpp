#include "AsyncObject.h"
#include "Log.h"
#include "BufferMgr.h"

#include <thread> 
#include <sys/time.h> 



static void AsyncThread(CAsyncObject* pAsyncObj)
{
	if(pAsyncObj == nullptr)
	{
		return ;
	}
	
	pAsyncObj->AsyncMsgThread();
}

class CAsyncObject* g_ciccAsyncObject = nullptr;

CAsyncObject::CAsyncObject(void):m_pAsyncObj(nullptr)
{
	g_ciccAsyncObject = this;
	m_pAsyncObj = this;
}

CAsyncObject::~CAsyncObject(void)
{
	if(!m_bStop)
		Terminate();
}


bool CAsyncObject::Init(int nThread/* = 1*/, CAsyncObject* pOwner/* = nullptr*/)
{
	for(int i = 0; i < nThread; ++i)
	{
		std::thread tr1(&AsyncThread, this);
		tr1.detach();
	}

	m_pAsyncObj = pOwner;
	return true;
}

bool CAsyncObject::PostMsg(PMSGHEADER pMsg)
{
	if(pMsg == nullptr)
		return false;

	CBuffer* pBuffer = g_pBufferMgr->GetBuffer(pMsg->dwLength + sizeof(MSGHEADER), __FILE__, __LINE__);
	if(nullptr == pBuffer)
	{
		return false;
	}
	//
	if(!pBuffer->Append(pMsg, pMsg->dwLength + sizeof(MSGHEADER)))
	{
		LogError("%s(%d) Append data fail.", __FILE__, __LINE__);
		return false;
	}
	//
	std::unique_lock<std::mutex> locker(m_clsLock);
	m_listMessage.push(pBuffer);
	m_condLock.notify_one();
	return true;
}

bool CAsyncObject::PostMsg(CBuffer* pBuffer)
{
	if(pBuffer == nullptr || 0 == pBuffer->GetBufLen())
	{
		return false;
	}
	//
	CBuffer* pBuf = g_pBufferMgr->GetBuffer(pBuffer->GetBufLen(), __FILE__, __LINE__);
	if(nullptr == pBuffer)
	{
		return false;
	}

	pBuf->Exchange(pBuffer);
	//
	std::unique_lock<std::mutex> locker(m_clsLock);
	m_listMessage.push(pBuf);
	m_condLock.notify_one();
	return true;
}

bool CAsyncObject::OnMsg(PMSGHEADER pMsg)
{
	if(m_pAsyncObj  == nullptr)
	{
		return false;
	}

	return m_pAsyncObj->OnMsg(pMsg);
}

void CAsyncObject::Terminate()
{
	if(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_bStop = true;
		m_condLock.notify_all();
	}
}

bool CAsyncObject::AsyncMsgThread()
{
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_condLock.wait(locker, [&]()->bool{ return !m_listMessage.empty(); });
		locker.unlock();
		while(true)
		{
			m_clsLock.lock();
			if(m_listMessage.empty())
			{
				m_clsLock.unlock();
				break;
			}
			auto pBuf = m_listMessage.front();
			m_listMessage.pop();
			m_clsLock.unlock();
			PMSGHEADER pMsg = (PMSGHEADER)pBuf->GetBufPtr();
			if(pMsg == nullptr)
				continue;
			auto bRet = OnMsg(pMsg);
			if(!bRet)
			{
				;//
			}
			//回收消息内存结构 
			g_pBufferMgr->ReleaseBuffer(pBuf);
		}
	}
	return true;
}

bool CAsyncObject::SetOwner(CAsyncObject* pOwner)
{
	if(pOwner == nullptr)
	{
		return false;
	}

	m_pAsyncObj = pOwner;

	return true;
}
	
bool CAsyncObject::SetTimer(PTIMERHEADER pTimer, int nSec)
{
	if(pTimer == nullptr)
	{
		return false;
	}

	return true;
}

bool CAsyncObject::SetTimerMili(PTIMERHEADER pTimer, int nMiliSec)
{
	if(nullptr == pTimer)
	{
		return false;
	}

	return true;
}


bool CAsyncObject::OnTimer(PTIMERHEADER pTimer)
{
	if(nullptr == pTimer)
	{
		return false;
	}

	return true; 
}

void CAsyncObject::ActiveTimerThread()
{
	while(!m_bTimerStop)
	{
		
		std::unique_lock<std::mutex> locker(m_clsTimerLock);
		std::cv_status nStatus = m_condTimer.wait_for(locker, std::chrono::microseconds(100));

		if(nStatus != std::cv_status::timeout)
		{
			break;
		}
		
		locker.unlock();
		struct timeval tv;
		gettimeofday(&tv, NULL);
		long long llTimer = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		while(!m_listTimer.empty())
		{
			m_clsTimerLock.lock();
			auto pBuf = m_listTimer.front();
			PTIMERHEADER pTimer = (PTIMERHEADER)pBuf->GetBufPtr();
			if(nullptr == pTimer)
			{
				m_listTimer.pop();
				m_clsTimerLock.unlock();
				continue;		
			}
			
			if(pTimer->llnTimerOver > llTimer)
			{
				m_listTimer.pop();
				//TODO 扔到线程池中解决
			}
			else 
			{
				m_clsTimerLock.unlock();
				break;
			}
			//	
			m_clsTimerLock.unlock();
		}
	}
}
