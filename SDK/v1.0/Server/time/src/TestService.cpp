#include "TestService.h"
#include "LogFile.h"




	
CTestService::CTestService()
{
}

CTestService:: ~CTestService()
{

}

bool CTestService::	OnInitialUpdate()
{
	if(!CObjService::OnInitialUpdate())
		return false ;
	LogInfo("Init Success.");

	return true;
}

bool CTestService::	OnTimeOut(struct tm* pTime)
{
	if(!CObjService::OnTimeOut(pTime))
		return false ;
	LogInfo("TimeOut");
}

bool CTestService::	OnSecondIdle()
{
	CObjService::OnSecondIdle();

	TIMERHEADER stTime = { 0 };
	stTime.dwTimerID = 1;
	SetTimerMilli(&stTime, 100);
	return true;
}

void CTestService::	OnTerminate()
{
	CObjService::OnTerminate();

}


bool CTestService::	OnTimer(PTIMERHEADER  pTimer)
{
	if(pTimer == nullptr)
		return false; 
	switch(pTimer->dwTimerID)
	{
		case 1:
			OnPrintTimer(pTimer);
			break;
		default:
			LogError("Not define the TimerID:%d", pTimer->dwTimerID);
			break;
	}
	return  true;
}


	
bool CTestService::OnPrintTimer(PTIMERHEADER pTimer)
{
	LogInfo("%s", __FUNCTION__);

	return true;
}
