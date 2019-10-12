
#include "AlarmService.h"




bool CAlarmService::OnInitialUpdate()
{
	__LOGINIT__;	

	LogInfo("Initial Success");
	
	return true;
}

bool CAlarmService::OnTimeout(struct tm* pTime)
{
	
	LogInfo("OnTimeOut...");

	return true;
}

bool CAlarmService::OnSecondIdle()
{
	
	return true;
}

bool CAlarmService::InvokeTerminate()
{
	LogInfo("InvokeTerminate...");
	return true;
}

bool CAlarmService::OnTerminate()
{
	LogInfo("OnTerminate...");
	return true;
}

