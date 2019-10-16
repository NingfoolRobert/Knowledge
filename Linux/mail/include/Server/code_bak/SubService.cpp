#include "SubService.h"

CSubService::CSubService()
{

}

CSubService::~CSubService()
{

}

bool CSubService::OnInitualUpdate()
{
	return true;
}

//BaseHandler* CreateHandler(int nType) = 0;

bool CSubService::OnTimeOut(struct tm* pTime)
{
	return true;
}

bool OnTerminate()
{
	return true;
}
