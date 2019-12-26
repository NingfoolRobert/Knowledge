#include "ActiveObject.h"

#include <sys/select.h> 
#include <sys/time.h> 




class CActiveObject* g_ActiveObject = nullptr;
	

void CActiveObject::ActiveTimerThread(CThread* pThread)
{
	while(true)
	{
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 100 * 1000;

		select(0, nullptr, nullptr, nullptr, &tv);
		
		gettimeofday(&tv, nullptr);
		long lTimerNow = tv.tv_sec * 1000 + tv.tv_usec  / 1000;
		while(true)
		{
			m_clsTimerLock.lock();
			auto it = m_listTimer.begin();
			if(it == m_listTimer.end())
			{
				m_clsTimerLock.unlock();
				break;
			}
			auto pTimer = *it;
			if(pTimer->lTimerOver <= lTimerNow)	
			{
				CBuffer* pBuf = nullptr;
				auto pTask = new CTimer(this, pBuf);
				if(pTask == nullptr)
				{
					//TODO LOG
					break;
				}
				AddTask(pTask);
				m_clsTimerLock.unlock();
				continue;
			}
			
			m_clsTimerLock.unlock();
			break;
		}

				
	}
}
