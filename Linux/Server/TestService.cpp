#include "TestService.h"



CTestService::CTestService()
{

}

CTestService::~CTestService()
{

}

bool CTestService::OnInitialUpdate()
{
	if(!CNetObjService::OnInitialUpdate())
		return false;
	

		
	return true;
}

bool CTestService::OnTimeout(struct tm* pTime)
{
	if(!CNetObjService::OnTimeout(pTime))
		return false;

	
	return true;
}

bool CTestService::InvokeTerminate()
{
	if(!CNetObjService::InvokeTerminate())
		return false;
	return true;
}

bool CTestService::OnTerminate()
{
	if(!CTestService::OnTerminate())
		return false;


	return true;
}



