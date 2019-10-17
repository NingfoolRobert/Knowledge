
#include "NetObjService.h"



CNetObjService::CNetObjService()
{

}

CNetObjService::~CNetObjService()
{

}

bool CNetObjService::OnInitialUpdate()
{
	if(!CSubService::OnInitualUpdate())
	{
		return false;
	}
	return true;
}

bool CNetObjService::OnTimeout(struct tm* pTime)
{
	if(!CSubService::OnTimeOut(pTime))
	{
		return false;
	}
	return false;
}

bool CNetObjService::OnSecondIdle()
{
	if(!CSubService::OnSecondIdle())
		return false;
	return true;
}

bool CNetObjService::InvokeTerminate()
{
	if(!CSubService::InvokeTerminate())
		return false;
	return true;
}

bool CNetObjService::OnTerminate()
{
	if(!CSubService::OnTerminate())
		return false;
	return true;
}

BaseHandler* CNetObjService::CreateHandler(int nType)
{
	return nullptr;
}



