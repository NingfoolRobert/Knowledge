
#include "TestNetService.h"

CTestService::CTestService()
{
	m_nPort = 30030;
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

	LogInfo("%s ", __FUNCTION__);
	
	return true;
}

bool CTestService::OnTimeOut(struct tm* pTime)
{
	if(!CNetObjService::OnTimeOut(pTime))
	{
		return false;
	}
	
	LogInfo("%s", __FUNCTION__);

	return true;
}

bool CTestService::OnSecondIdle()
{
	if(!CNetObjService::OnSecondIdle())
	{
		return false;
	}
	LogInfo("%s ", __FUNCTION__);
	
	return true;
}

bool CTestService::InvokeTerminate()
{
	if(!CNetObjService::InvokeTerminate())
	{
		return false;
	}
	return true;
}

void CTestService::OnTerminate()
{
	CNetObjService::OnTerminate();
	
	LogInfo("%s", __FUNCTION__);
}

CUserObject* CTestService::CreateUserObject(int nOrigine)
{
	return nullptr;
}
