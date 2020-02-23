
#include "TestNetService.h"

CTestService::CTestService()
{
	m_nPort = 30031;
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

	if(!m_connection.InitialUpdate("127.0.0.1", 30030))
	{
		LogError("init fail.");
	}

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
	//
	if(++m_nCount < 10)
	{
		return true;
	}
	m_nCount = 0;
	//
	
	HEADER stHeader;
	memset(&stHeader, 0, sizeof(stHeader));
	stHeader.dwType = 1;
	stHeader.wOrigin = 1;
	
	bool bRet = m_connection.SendMsg(&stHeader);
	if(!bRet)
	{
		LogError("Send msg fail.");
	}
	
	m_connection.Terminate();
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
