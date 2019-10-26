#include "ObjectService.h"


CObjectService::CObjectService()
{

}

CObjectService::~CObjectService()
{

}


bool CObjectService::OnInitialUpdate()
{
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


