
#include "NetObjService.h"
#include "Log.h"
#include "NetClient.h"


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
		LogError("");
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
	
	auto pUserObject = 	OnCreateUserObject(pMsg->wOrigin);
	if(nullptr == pUserObject)
	{
		LogError("%s(%d) Create UserObject is NULL. wOrigin: %d", __FILE__, __LINE__, pMsg->wOrigin);		
		return false;
	}

	pNetClient->BindUserObj(pUserObject);

	OnNetConnect(pNetClient);
	
	pNetClient->OnMsg(pMsg);
	
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

