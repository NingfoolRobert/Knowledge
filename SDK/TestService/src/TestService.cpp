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
	
	

	return true;
}

bool CTestService::OnTimeOut(struct tm* pTime)
{
	if(!CObjService::OnTimeOut(pTime))
	{
		return false;
	}

	return true;
}

bool CTestService::OnSecondIdle()
{
	if(!CObjService::OnSecondIdle())
	{
		return false;
	}
	return true;
}

void CTestService::OnTerminate()
{
	CObjService::OnTerminate();
}
