#include "TestService.h"
#include "Version"

CTestService::CTestService()
{

}

CTestService::~CTestService()
{

}


bool CTestService::OnInitialUpdate()
{
	if(!CNetObjService::OnInitialUpdate())
	{
		return false;
	}


	return true;
}

bool CTestService::OnTimeOut(struct tm* pTime)
{
	if(!CNetObjService::OnTimeOut(pTime))
	{
		return false;
	}

	return true;
}

bool CTestService::OnSecondIdle()
{
	CNetObjService::OnSecondIdle();
	
	return true;
}

bool CTestService::InvokeTerminate()
{
	CNetObjService::InvokeTerminate();

	return true;
}

void CTestService::OnTerminate()
{
	CNetObjService::OnTerminate();
	
}
	
void CTestService::OnRegisterVersion()
{
	strcpy(m_szVersion, SERVICE_VERSION_INFO);
}

CUserObject* CTestService::OnCreateUserObject(int nOrigine)
{
	return nullptr;
}

void CTestService::OnRegisterObject()
{

}
