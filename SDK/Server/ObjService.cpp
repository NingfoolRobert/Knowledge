#include "ObjService.h"




bool CObjService::OnInitialUpdate()
{
	if(!CObjectService::OnInitialUpdate())
	{
		return false;
	}

	return true;
}

bool CObjService::OnTimeOut(struct tm* pTime)
{
	if(!CObjectService::OnTimeOut(pTime))
	{
		return false;
	}
	return true;
}

bool CObjService::OnSecondIdle()
{
	return true;
}

void CObjService::OnTerminate()
{
	CObjectService::OnTerminate();
}
