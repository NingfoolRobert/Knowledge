#include "TestService.h"


CTestService::CTestService(void)
{
}

CTestService::~CTestService(void)
{
}

bool CTestService::OnInitialUpdate()
{
	if(!CObjService::OnInitialUpdate())
	{
		return false;
	}
	
	LogInfo("InitialUpdate Success.");	

	LogFatal("InitialUpdate");
	return true;
}

bool CTestService::OnTimeOut(struct tm* pTime)
{
	if(!CObjService::OnTimeOut(pTime))
	{
		return false;
	}

	LogFatal("Time Out %d:%d", pTime->tm_hour, pTime->tm_min);
	return true;
}

bool CTestService::OnSecondIdle()
{
	if(!CObjService::OnSecondIdle())
	{
		return false;
	}
	
	LogInfo("Second info");
	return true;
}

void CTestService::OnTerminate()
{
	CObjService::OnTerminate();
}
	
CUserObject* CTestService::OnCreateUserObject(int nOrgine)
{
	return nullptr;
}
