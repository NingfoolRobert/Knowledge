#include "NetObjService.h"

CNetObjService::CNetObjService(void)
{
}

CNetObjService::~CNetObjService()
{
}


bool CNetObjService::OnInitialUpdate()
{
	//
	if(!CObjectService::OnInitialUpdate())
	{
		return false;
	}
	
	if(!CNetService::OnInitualUpdate())
	{
		LogError("Net Init fail.");
		return false;
	}
		
	return true;
}

bool CNetObjService::OnTimeOut(struct tm* pTime)
{
	if(!CNetService::OnTimeOut(pTime))
	{
		return false;
	}
	//
	if(!CObjectService::OnTimeOut(pTime))
	{
		return false;
	}

	return true;
}

bool CNetObjService::OnSecondIdle()
{
	CNetService::OnSecondeIdle();

//	CObjectService::OnSecondIdle();
	
	return true;
}

bool CNetObjService::InvokeTerminate()
{
	CObjectService::InvokeTerminate();	
	return true;
}

void CNetObjService::OnTerminate()
{
	CNetService::OnTerminate();
	//
	CObjectService::OnTerminate();
}
 
CUserObject* CNetObjService::OnNetUserObject(PHEADER pHeader)
{
	return OnCreateUserObject(pHeader->wOrigin);
}
	
CUserObject* CNetObjService::OnCreateUserObject(int nOrigin)
{
	return nullptr;
}
