#include "ObjService.h"

bool CObjService::OnInitialUpdate()
{
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
	if(!CObjectService::OnSecondIdle())
	{
		return false;
	}
	return true;
}

bool CObjService::OnTerminate()
{
	CObjectService::OnTerminate();
	return true;
}

bool CObjService::OnMsg(PMSGHEADER pMsg)
{
	return true;
}

bool CObjService::OnRegisterObject()
{
	return true;
}
