
#include "Service.h"
#include <signal.h>
#include <string.h>
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono> 

#include "version.h"

using namespace std;

//static void TimeOut(CService* pService)
//{
//	if(nullptr != pService)
//	{
//		time_t tNow = time(nullptr);
//		struct tm pTime;// = nullptr;
//		localtime_r(&tNow, &pTime);
//		pService->OnTimeOut(&pTime);
//	}
//}
//static void SecondeIdle(CService* pService)
//{
//	if(nullptr != pService)
//	{
//		pService->OnSecondIdle();
//	}
//}

std::atomic<int> nTimeCount(0);

class CService* g_Service = nullptr;

//void TimerHandle(int sino)
//{
//	if(g_Service)
//	{
//		std::thread tr1(&SecondeIdle,g_Service);
//		tr1.detach();
//
//		if(nTimeCount == 60)
//		{
//			std::thread  tr2(&TimeOut, g_Service);
//			tr2.detach();
//			nTimeCount = 0;
//		}
//		nTimeCount++;
//	}
//}
//

CService::CService()
{	
	g_Service = this;
	m_tStart = time(nullptr);
}

CService::~CService()
{
//	Terminate();
}

bool CService::Execute(const char* pszCommand, const char* pszFileName /*= ""*/, int nLine/*= 0*/)
{
	if(pszCommand != nullptr || 0 != strlen(pszCommand) )
	{
		if(strncasecmp(pszCommand, "-V",2) == 0)
		{
			printf("Version:	%s\nDate:		%s %s\n ",_VERSION_, __DATE__, __TIME__);
		}
		else if(strncasecmp(pszCommand, "--version", 9) == 0)
		{	
			printf("Version:	%s\nDate:		%s %s\n ",_VERSION_, __DATE__, __TIME__);
		}
		else 
		{
			printf("-help\n	-V/--version: Output the service version No.");
		}
		return 0;
	}
//	__LOGINIT__;
	LogInfo("Init Service ...");
	if(!OnInitialUpdate())
	{
		LogError("%s(%d) Init Service fail....", __FILE__, __LINE__);
		exit(-1);
		return false;
	}

//	if(signal(SIGALRM, TimerHandle) == SIG_ERR)
//	{
//		//TODO  log 
//		exit(-1);
//		return false;
//	}
//	struct itimerval tv;
//	tv.it_value.tv_sec = 0;
//	tv.it_value.tv_usec = 500000;
//	tv.it_interval.tv_sec = 1 ;
//	tv.it_interval.tv_usec = 0;
//	if(setitimer(ITIMER_REAL, &tv, NULL) != 0)
//	{
//		return -1;
//	}
	m_bStop = false;	
	
//	struct timeval tMin;
//	tMin.tv_sec = 1;
//	tMin.tv_usec = 0;
	
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		std::cv_status nStatus = m_condStop.wait_for(locker, std::chrono::seconds(1));
		if(nStatus != std::cv_status::timeout)
		{
			InvokeTerminate();
			break;
		}
	
		time_t tNow = time(nullptr);
		struct tm pTime;// = nullptr;
		localtime_r(&tNow, &pTime);
		OnSecondIdle();
		if(nTimeCount == 60)
		{
			OnTimeOut(&pTime);
			nTimeCount = 0;
		}
		nTimeCount++;	
	}

	OnTerminate();
	return true;
}

//
void CService::Terminate()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	m_condStop.notify_all();
	m_bStop = true;
	LogInfo("Service Stop....");
}
