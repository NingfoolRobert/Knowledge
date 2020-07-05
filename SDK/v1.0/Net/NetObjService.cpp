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
	
bool CNetObjService::OnNetMsg(CNetClient* pNetClient, PHEADER pHeader)
{
	if(nullptr == pNetClient || nullptr == pHeader)
		return false; 
	if(pNetClient->IsBindUserObject())
	{
		return pNetClient->OnMsg(pHeader);
	}

	CUserObject* pUserObj = OnCreateUserObject(pHeader->wOrigin);
	if(nullptr != pUserObj)
	{
		pNetClient->BindUserObject(pUserObj);
		pNetClient->SetMsgOrgin(pHeader->wOrigin);
		ADD_REF(pUserObj);
		pUserObj->m_dwIP	= pNetClient->m_dwIP;
		pUserObj->m_nPort	= pNetClient->m_nPort;

		pUserObj->OnConnect();
		return pUserObj->OnMsg(pHeader);
	}
	
	return false;
}
