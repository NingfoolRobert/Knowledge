#include "TestService.h"


CTestService::CTestService()
{
	m_nPort = 30030;
}

CTestService::~CTestService()
{

}


bool CTestService::OnInitialUpdate()
{
	if(!CNetService::OnInitialUpdate())
	{
		return false;
	}
	
	return true;
}

bool CTestService::OnTimeOut(struct tm* pTime)
{
	if(!CNetService::OnTimeOut(pTime))
	{
		return false;
	}
	return true;
}

bool CTestService::OnSecondIdle()
{
	if(!CNetService::OnSecondIdle())
	{
		return false;
	}
	return true;
}
	
bool CTestService::InvokeTerminate()
{
	return true;
}

bool CTestService::OnTerminate()
{
	if(!CNetService::OnTerminate())
	{
		return false;
	}
	return true;
}

bool CTestService::OnRegisterObject()
{
	
	return true;
}

CUserObject* CTestService::OnCreateUserObject(int nOrigine)
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
