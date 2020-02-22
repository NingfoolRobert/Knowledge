#include "ObjectService.h"


CObjectService::CObjectService(void)
{

}

CObjectService::~CObjectService(void)
{

}

bool CObjectService::OnInitialUpdate()
{
	//Load Ini configure 
	if(!CActiveObject::Init(3))
	{
		return false;
	}


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
