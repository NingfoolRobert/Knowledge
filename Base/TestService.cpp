#include "TestService.h"


bool CTestService::OnInitialUpdate()
{
	return true;
}

bool CTestService::OnTimeOut(struct tm* pTime)
{
	return true;
}

bool CTestService::OnSecondIdle()
{
	return true;
}

bool CTestService::OnTerminate()
{
	return true;
}

bool CTestService::OnRegisterObject()
{
	return true;
}

CUserObject* OnCreateUserObject(int nOrigine)
{
	switch(nOrigine)
	{
		case 1:
			return nullptr;
		default:
			break;
	}
	return nullptr;
}
