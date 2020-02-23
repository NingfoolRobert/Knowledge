#include "ObjectService.h"

#define WORKTHREAD_COUNT_MULTI	4

#define WORKTHREAD_TASK_COUNT_MULTI		50

CObjectService::CObjectService(void):m_cnWorkThread(2)
{

}

CObjectService::~CObjectService(void)
{

}

bool CObjectService::OnInitialUpdate()
{
	//Load Ini configure
	if(!CActiveObject::Init(m_cnWorkThread * WORKTHREAD_COUNT_MULTI, m_cnWorkThread, m_cnWorkThread * WORKTHREAD_TASK_COUNT_MULTI))
	{
		return false;
	}

	OnRegisterObject();

	LogInfo("Async Object Init Success...");

	return true;
}

bool CObjectService::OnTimeOut(struct tm *pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}

	if(pTime->tm_min % 30 == 0)
	{
		g_pBufferMgr->ClearAllBuffer();
	}

	CBuffer stBuf;
	g_pBufferMgr->PrintInfo(&stBuf);

	LogInfo("%s", stBuf.GetBufPtr());

	CActiveObject::PrintfInfo();
	return true;
}

bool CObjectService::OnSecondIdle()
{
	return true;
}

bool CObjectService::InvokeTerminate()
{
	return true;
}

void CObjectService::OnTerminate()
{
	CActiveObject::Terminate();
}
	
void CObjectService::OnRegisterObject()
{
	
}
