#include "Timer.h"
#include <sys/time.h> 
#include <thread>
#include <unistd.h> 
#include "LogFile.h"
#include "BufferMgr.h"


CTimer::CTimer()
{
}

CTimer::~CTimer()
{
	
}
	
bool CTimer::SetTimer(PTIMERHEADER pTimer, int nSec)
{
	return SetTimerMilli(pTimer, nSec * 1000);
}

bool CTimer::SetTimerMilli(PTIMERHEADER pTimer, int nMilliSec)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	if(!m_bEnableTimer && m_listTimer.size() == 0)
	{
		m_bEnableTimer = true;
		std::thread tr1(&CTimer::ActiveTimerFunc, this);
		tr1.detach();
	}
	if(nullptr == pTimer)
		return false; 

	struct timeval tNow;
	gettimeofday(&tNow, nullptr);
	
	CBuffer* pBuf = g_pBufferMgr->GetBuffer(sizeof(TIMERHEADER) + pTimer->dwLength);
	if(nullptr == pBuf)
	{
		LogError("memory error");
		return false; 
	}
	
	if(!pBuf->Append(pTimer, sizeof(TIMERHEADER) + pTimer->dwLength))
	{
		LogError("Append data fail.");
		return false;
	}

	PTIMERHEADER pTimer1 = (PTIMERHEADER) pBuf->GetBufPtr();
	pTimer1->tExpire = tNow.tv_sec * 1000LL + tNow.tv_usec / 1000 + nMilliSec;

	m_listTimer.push(pBuf);
	return true;
}

bool CTimer::KillTimer(unsigned int dwTimerID)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	std::vector<CBuffer*> listTmp;
	
	CBuffer* pBuf = nullptr;
	PTIMERHEADER pTimer = nullptr;
	while(!m_listTimer.empty())
	{
		pBuf = m_listTimer.top();
		m_listTimer.pop();
		if(nullptr == pBuf) continue;
		pTimer = (PTIMERHEADER) pBuf->GetBufPtr();
		if(nullptr == pTimer || pTimer->dwTimerID != dwTimerID)
		{
			listTmp.push_back(pBuf);
		}
		else 
		{
			g_pBufferMgr->ReleaseBuffer(pBuf);
		}
	}
	
	for(auto i = 0u; i < listTmp.size(); ++i)
		m_listTimer.push(listTmp[i]);

	return true;
}

void CTimer::ActiveTimerFunc()
{
	PTIMERHEADER pTimer = nullptr;
	struct timeval tmNow;
	long long tNow;
	while(!m_bStop)
	{
		usleep(10 * 1000);
		gettimeofday(&tmNow, nullptr);
		tNow =(tmNow.tv_sec) * 1000LL  + tmNow.tv_usec / 1000;
		while(!m_listTimer.empty())
		{
			m_clsLock.lock();
			if(m_listTimer.empty())
			{
				m_clsLock.unlock();
				break;
			}
			
			CBuffer* pBuffer = m_listTimer.top();
			pTimer = (PTIMERHEADER)pBuffer->GetBufPtr();
			if(tNow < pTimer->tExpire)
			{
				m_clsLock.unlock();
				break;
			}
			//
			m_listTimer.pop();
			m_clsLock.unlock();
			OnTimer(pTimer);
			g_pBufferMgr->ReleaseBuffer(pBuffer);
		}

	}
}
