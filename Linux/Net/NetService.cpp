#include "NetService.h"

bool CNetService::OnInitialUpdate()
{
	return true;
}
bool CNetService::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
		return false;
	return true;
}
bool CNetService::OnSecondIdle()
{
	return true;
}
bool CNetService::OnInvokeTerminate()
{
	return true;
}
bool CNetService::OnTerminate()
{
	return true;
}

CNetClient*	CNetService::CreateNetClient()
{
	return nullptr;
}

bool OnNetConnect(CNetClient* pNetClient)
{
	if(pNetClient == nullptr)
		return false;
	pNetClient->OnConnect();
	return true;
}
bool OnNetBreak(CNetClient* pNetClient)
{
	if(pNetClient == nullptr)
		return false;
	return true;
}
bool OnNetTickCount(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
		return false;

	return true;
}

bool CNetService::AddClient(CNetClient* pNetClient)
{
	if(pNetClient == nullptr)
		return false;
	return true;
}
bool CNetService::OnAccept()
{
	return true;
}

bool CNetService::Accept(int fd, int nEnterPort, bool bEpoll = true)
{
	
	return true;
}


