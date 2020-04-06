#include "ObjectService.h"

	
CObjectService::CObjectService()
{

}

CObjectService::~CObjectService()
{

}

bool CObjectService::OnInitialUpdate()
{
	int cnThread = 10;
	if(CActiveObject::Init(cnThread))
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
	
bool CObjectService::OnEvent(PEVENTHEADER pEvent)
{
	if(nullptr == pEvent)
		return false;
	return true;
}

void CObjectService::RegisterObject()
{

}

CUserObject* CObjectService::CreateUserObject(int nOrigine)
{
	return OnCreateUserObject(nOrigine);
}
