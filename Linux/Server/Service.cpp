
#include "Service.h"
#include <signal.h>
#include <string.h>
#include <iostream>
#include <atomic>

atomic<int> nTimeCount(0);

class CService* g_Service = nullptr;

static void TimerHandle(int sino)
{
	time_t tNow = time(nullptr);
	struct tm* pTime;
	localtime_r(&tNow, pTime);
	
	if(g_Service)
	{
		g_Service->OnSecondIdle();

		if(nTimeCount == 60)
		{
			g_Service->OnTimeout(pTime);
			nTimeCount = 0;
		}
		nTimeCount++;
	}
}


CService::CService()
{	
	g_Service = this;
}

CService::~CService()
{

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
	

	return true;
}
