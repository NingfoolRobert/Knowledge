

#include "NetObjService.h"




bool CNetObjService::OnInitialUpdate()
{
	return true;
}

bool CNetObjService::OnTimeout(struct tm* pTime)
{
	if(!__super::OnTimeout(pTime))
		return false;

	return true;
}

bool CNetObjService::OnSecondIdle()
{
	return true;
}

bool CNetObjService::InvokeTerminate()
{
	return true;
}

bool CNetObjService::OnTerminate()
{
	return true;
}
