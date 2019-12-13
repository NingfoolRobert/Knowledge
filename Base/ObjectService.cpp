#include "ObjectService.h"


CObjectService::CObjectService()
{

}

CObjectService::~CObjectService()
{

}


bool CObjectService::OnInitialUpdate()
{
	if(!CAsyncObject::Init(3, this))
	{
		return false;
	}
	return true;
}

bool CObjectService::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}
	return true;
}

bool CObjectService::OnSecondIdle()
{
	return true;
}

bool CObjectService::OnTerminate()
{
	return true;
}

bool CObjectService::OnRegisterObject()
{
	return true;
}


bool CObjectService::OnMsg(PMSGHEADER pAsyncMsg)
{
	if(nullptr == pAsyncMsg)
	{
		return false;
	}
	return true;
}

bool CObjectService::PostAsyncMsg(PMSGHEADER pMsg)
{
	if(nullptr == pMsg)
	{
		return false;
	}
	
	return CAsyncObject::PostMsg(pMsg);
}

bool CObjectService::PostAsyncMsg(CBuffer* pBuffer)
{
	if(nullptr == pBuffer)
	{
		return false;
	}
	return CAsyncObject::PostMsg(pBuffer);
}
