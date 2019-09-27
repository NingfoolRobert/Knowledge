
#include "ActiveObject.h"
#include "ThreadPool.h"
#include <sys/select.h>
#include <time.h> 
#include <sys/time.h> 



#define TIMER_PRECISION			100		//100毫秒

static void ActiveTimer(CActiveObject* pObj)
{
	if(pObj == nullptr)
	{
		//TODO 
		return;
	}

	time_t tNow = time(NULL);
	struct tm* pTime ;
	localtime_r(&tNow, pTime);
	TIMERHEADER stTime = m_TimerTask.front();
	

}

void  CActiveObject::TimerThread(CActiveObject* pObj)
{
	if(nullptr == pObj)
	{
		//TODO 
		return ;
	}	
	//
	
	int nSecond = TIMER_PRECISION  / 1000000;
	int uSecond = TIMER_PRECISION  % 1000000;
	
	struct timeval tm;
	tm.tv_sec = nSecond;
	tm.tv_usec = uSecond;
	while(!m_bStop)
	{
		select(0,NULL,NULL,NULL,&tm);
		
	}
}

CActiveObject::CActiveObject()
{

}

CActiveObject::~CActiveObject()
{
	
}
bool CActiveObject::Init(int cnWorkThread)
{
//	m_ThreadPool.init(cnWorkThread);

	_THREADPOOL_INIT_(cnWorkThread);
	
	return true;
}

bool CActiveObject::OnTimeout(struct tm* pTime)
{
	return true;
}

bool CActiveObject::OnSecondIdle()
{
	return true;
}
bool CActiveObject::InvokeTerminate()
{
	return true;
}

bool CActiveObject::OnTerminate()
{
	InvokeTerminate();
	m_bStop = true;	
	return true;
}


