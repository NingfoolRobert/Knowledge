
#include "Service.h"
#include <signal.h>
#include <string.h>
#include <iostream>
#include <atomic>
#include <thread>


using namespace std;

static void TimeOut(CService* pService)
{
	if(nullptr != pService)
	{
		time_t tNow = time(nullptr);
		struct tm pTime;// = nullptr;
		localtime_r(&tNow, &pTime);
		pService->OnTimeOut(&pTime);
	}
}
static void SecondeIdle(CService* pService)
{
	if(nullptr != pService)
	{
		pService->OnSecondIdle();
	}
}

std::atomic<int> nTimeCount(0);

class CService* g_Service = nullptr;

void TimerHandle(int sino)
{
	if(g_Service)
	{
		std::thread tr1(&SecondeIdle,g_Service);
		tr1.detach();

		if(nTimeCount == 60)
		{
			std::thread  tr2(&TimeOut, g_Service);
			tr2.detach();
			nTimeCount = 0;
		}
		nTimeCount++;
	}
}


CService::CService()
{	
//	m_bStop = false;
	g_Service = this;
	m_tStart = time(nullptr);
}

CService::~CService()
{
//	Terminate();
}

bool CService::Execute(const char* pszFileName /*= ""*/, int nLine/*= 0*/)
{
//	__LOGINIT__;
	LogInfo("Init Service ...");
	if(!OnInitialUpdate())
	{
		//TODO  log init  fail...
		//
		LogWarn("%s(%d) Init Service fail....", __FILE__, __LINE__);
		exit(-1);
		return false;
	}

	if(signal(SIGALRM, TimerHandle) == SIG_ERR)
	{
		//TODO  log 
		exit(-1);
		return false;
	}
	struct itimerval tv;
	tv.it_value.tv_sec = 0;
	tv.it_value.tv_usec = 500000;
	tv.it_interval.tv_sec = 1 ;
	tv.it_interval.tv_usec = 0;
	if(setitimer(ITIMER_REAL, &tv, NULL) != 0)
	{
		return -1;
	}
	m_bStop = false;	
	while(!m_bStop);
	{
		;
//		select(0,NULL,NULL,NULL,)
	}
	//Terminate();
	InvokeTerminate();
	OnTerminate();
	return true;
}

//
void CService::Terminate()
{
//	InvokeTerminate();
	m_bStop = true;
//	OnTerminate();
	LogInfo("Service Stop....");
}
