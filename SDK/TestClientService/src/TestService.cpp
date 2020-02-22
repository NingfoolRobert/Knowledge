#include "TestService.h"


CTestService::CTestService(void)
{
	m_nPort = 30030;
}

CTestService::~CTestService(void)
{
}

bool CTestService::OnInitialUpdate()
{
	if(!CNetObjService::OnInitialUpdate())
	{
		return false;
	}
	
	LogInfo("InitialUpdate Success.");	

	char szIP[32] = { "127.0.0.1" };
	int nPort = 30030;
		
//	m_Connection.InitialUpdate(szIP, nPort);
	return true;
}

bool CTestService::OnTimeOut(struct tm* pTime)
{
	if(!CNetObjService::OnTimeOut(pTime))
	{
		return false;
	}

	EVENTHEADER stEvent;
	memset(&stEvent, 0, sizeof(stEvent));
	
	stEvent.dwType = 1;


	PostEvent(&stEvent);

	
	LogInfo("Time out ");
	return true;
}

bool CTestService::OnSecondIdle()
{
	if(!CNetObjService::OnSecondIdle())
	{
		return false;
	}
	
	LogInfo("Second info");
	return true;
}

void CTestService::OnTerminate()
{
	CNetObjService::OnTerminate();
}

bool CTestService::OnEvent(PEVENTHEADER pEvent)
{
	if(nullptr == pEvent)
		return false;

	switch(pEvent->dwType)
	{
		case 1:
			OnSendEvent(pEvent);
		break;
		default:
			break;
	}
	return true;
}

bool CTestService::OnTimer(PTIMERHEADER pTimer)
{
	if(nullptr == pTimer)
	{
		return false;
	}
	
	switch(pTimer->dwTimerID)
	{
		case 1:
			OnSendTimer(pTimer);
		default:
			break;
	}

	return true;
}

CUserObject* CTestService::CreateUserObject(int nOrigine)
{
	return nullptr;
}

bool CTestService::OnSendEvent(PEVENTHEADER pEvent)
{
	HEADER stHeader;
	memset(&stHeader, 0, sizeof(stHeader));
	stHeader.dwType = 10;
	stHeader.wOrigin = 1;
	
	bool bRet = false; // m_Connection.SendMsg(&stHeader);
	if(bRet)
	{
		LogInfo("Send Msg Success.");
	}
	else 
	{
		LogError("Send Msg fail.");
	}


	TIMERHEADER stTimer;
	memset(&stTimer, 0, sizeof(stTimer));
	stTimer.dwTimerID =  1;

	SetTimer(&stTimer, 1);


	return true;
}

bool CTestService::OnSendTimer(PTIMERHEADER pTimer)
{
	LogInfo("%s", __FUNCTION__);
	return true;
}
