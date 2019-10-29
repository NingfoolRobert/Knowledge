
#include "NetObjService.h"
#include "Log.h"
#include "UserObject.h"

CNetObjService::CNetObjService()
{

}

CNetObjService::~CNetObjService()
{

}

bool CNetObjService::OnInitialUpdate()
{
	if(!CNetService::OnInitialUpdate())
	{
		return false;
	}
	OnRegisterObject();

	return true;
}

bool CNetObjService::OnServiceBegin()
{
	return true;
}

bool CNetObjService::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}
	return true;
}

bool CNetObjService::OnSecondIdle()
{
	return true;
}

bool CNetObjService::OnTerminate()
{
	return true;
}


bool CNetObjService::OnNetConnect(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
	{
		LogError("NetClient is NULL.");
		return false;
	}
	pNetClient->OnConnect();
	return true;
}

bool CNetObjService::OnNetMsg(CNetClient* pNetClient, PHEADER pMsg)
{

	if(nullptr == pNetClient || nullptr == pMsg)
	{
		return false;
	}
	
	if(!pNetClient->IsBindUserObject())
	{

		CUserObject* pUserObject = OnCreateUserObject(pMsg->wOrigin);
		if(nullptr == pUserObject)
		{
			LogError("%s(%d) Create UserObject is NULL. wOrigin: %d", __FILE__, __LINE__, pMsg->wOrigin);		
			return false;
		}
		
		pUserObject->BindNetClient(pNetClient);
		pUserObject->SetPeerAddr(pNetClient->m_dwIP, pNetClient->m_nPort);
		pNetClient->BindUserObj(pUserObject);
		//设置对象基本信息  IP Port
		//
		OnNetConnect(pNetClient);
	}
	
	return true;
}

bool CNetObjService::OnNetBreak(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
	{
		return false;
	}
	return true;
}

bool CNetObjService::OnNetTickCount(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
	{
		return false;
	}
	return true;
}


bool CNetObjService::OnRegisterObject()
{
	return true;
}


CUserObject* CNetObjService::OnCreateUserObject(int nOrigine)
{
	return nullptr;
}

