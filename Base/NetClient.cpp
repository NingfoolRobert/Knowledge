#include "NetClient.h"
	

CNetClient::CNetClient()
{

}

CNetClient::~CNetClient()
{
	
}


bool CNetClient::OnInitialUpdate(int fd, CNetIOMgr* pMgr)		//初始化Socket, UserObject, 
{
	return true;
}

bool CNetClient::OnTimeOut(struct tm* pTime)
{
	return true;
}


bool CNetClient::OnSecondIdle()
{
	return true;
}


bool CNetClient::OnTerminate()
{
	return true;
}


