#include "Timer.h"
#include "ThreadPool.h"



CTimer::CTimer()
{
}

CTimer::~CTimer()
{

}

void CTimer::Update(struct timeval* pTv)
{
	auto it = m_listTimer.begin();
	for(;it != m_listTimer.end(); ++it)
	{
		TIMERHEAD tHead = it->first;
		if(tHead.unExpireSec >= pTv->tv_sec && tHead.unExpireUsec >= pTv->tv_usec)
		{
			ASYNC_HANDLE(it->second);
		}
		
	}
}
