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

bool CNetClient::OnMsg(PHEADER pMsg)		//消息到达网络端口
{
	if(nullptr == pMsg)
	{
		return false;
	}
	return true;
}

bool CNetClient::OnConnect()				//链接消息网络接口
{
	return true;
}

bool CNetClient::OnBreak()					//网络断开
{
	return true;
}

