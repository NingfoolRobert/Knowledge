
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
		struct tm* pTime = nullptr;
		localtime_r(&tNow, pTime);
		pService->OnTimeout(pTime);
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

static void TimerHandle(int sino)
{
	time_t tNow = time(nullptr);
	struct tm* pTime;
	localtime_r(&tNow, pTime);
	
	if(g_Service)
	{
		std::thread tSecond(&Second,g_Service);
		if(nTimeCount == 60)
		{
			std::thread  tr2(&Timeout, g_Service);
			min.detach();
			nTimeCount = 0;
		}
		nTimeCount++;
	}
}


CService::CService()
{	
	m_bStop = false;
	g_Service = this;
}

CService::~CService()
{
	m_bStop = true;
}

bool CService::Excute(const char* pszFileName /*= ""*/, int nLine/*= 0*/)
{

	if(!OnInitialUpdate())
	{
		//TODO  log init  fail...
		exit(-1);
		return false;
	}

	if(signal(SIGALRM, TimerHandle) == SIG_ERR)
	{
		//TODO  log 
		exit(-1);
		return false;
	}
	while(!m_bStop);

	Terminate();
	return true;
}
bool CService::Terminate()
{
	InvokeTerminate();
	m_bStop = true;
	OnTerminate();
	return true;
}
