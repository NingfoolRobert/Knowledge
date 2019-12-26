#include "ObjectService.h"



CObjectService::CObjectService(void)
{

}

CObjectService::~CObjectService(void)
{

}

bool CObjectService::OnInitialUpdate()
{
	return true;
}

bool CObjectService::OnTimeOut(struct tm *pTime)
{
	return true;
}

bool CObjectService::OnSecondIdle()
{
	return true;
}

void CObjectService::OnTerminate()
{

}
