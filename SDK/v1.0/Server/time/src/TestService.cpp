#include "TestService.h"
#include "LogFile.h"
#include <malloc.h>



	
CTestService::CTestService()
{
	m_nFlag  = 1;
}

CTestService:: ~CTestService()
{

}

bool CTestService::	OnInitialUpdate()
{
	if(!CObjService::OnInitialUpdate())
		return false ;

	int* ptr = new int;
	*ptr = 2;
	
	int* p = ptr;
	ptr = nullptr;
	LogInfo("%d", *p);


	LogInfo("Init Success.");

	return true;
}

bool CTestService::	OnTimeOut(struct tm* pTime)
{
	if(!CObjService::OnTimeOut(pTime))
		return false ;

	if(m_listInfo.size() == 0 && m_nFlag == 1)
	{
		for(auto i = 0; i < 100000; ++i)
		{
			PTESTINFO pTest = new TESTINFO;
		
			m_listInfo.push_back(pTest);
			PINFO pInfo = new INFO;
			m_listval.push_back(pInfo);
		}
		LogInfo("Create Success!");
		m_nFlag = 2;
		return true;
	}


	bool bDel = false;
	if(m_nFlag == 2)
	{
		for(auto i = 0u; i < m_listInfo.size(); ++i)
		{
			PTESTINFO pInfo = m_listInfo[i];
			delete pInfo;
			pInfo = nullptr;
		}
		for(auto i = 0u; i < m_listval.size() - 200; ++i)
		{
			PINFO pInfo = m_listval[i];
			delete pInfo;	
			pInfo = nullptr;
		}

		m_listInfo.clear();
		bDel = true;
		LogInfo("Delete ");
		m_nFlag = 3;
		return true;
	}

	if(m_nFlag == 3)
	{
		int nRet = malloc_trim(0);
		LogInfo("memory Reclaim Success， %d", nRet);
		m_nFlag = 1;
	}


	LogInfo("TimeOut");
}

bool CTestService::	OnSecondIdle()
{
	CObjService::OnSecondIdle();

	TIMERHEADER stTime = { 0 };
	stTime.dwTimerID = 1;
//	SetTimerMilli(&stTime, 100);
	return true;
}

void CTestService::	OnTerminate()
{
	CObjService::OnTerminate();

}


bool CTestService::	OnTimer(PTIMERHEADER  pTimer)
{
	if(pTimer == nullptr)
		return false; 
	switch(pTimer->dwTimerID)
	{
		case 1:
			OnPrintTimer(pTimer);
			break;
		default:
			LogError("Not define the TimerID:%d", pTimer->dwTimerID);
			break;
	}
	return  true;
}


	
bool CTestService::OnPrintTimer(PTIMERHEADER pTimer)
{
	LogInfo("%s", __FUNCTION__);

	return true;
}
