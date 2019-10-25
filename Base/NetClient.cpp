#include "NetClient.h"
	

CNetClient::CNetClient()
{
	m_dwRecvSerial = 0;
	m_dwSendSerial = 0;
	m_pbufRecv = nullptr;
	m_pbufSend = nullptr;
}

CNetClient::~CNetClient()
{
	
}


bool CNetClient::OnInitialUpdate(int fd)		//初始化Socket, UserObject, 
{	
	Attach(fd);

//	char szAddress[32] = { 0 };

	struct sockaddr PeerAddr;
	socklen_t socklen = sizeof(PeerAddr);
	GetPeerName(PeerAddr, socklen);
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

void CNetClient::OnRecv()		
{
	//一次最多收缓冲区大小的数据 

	
}

void CNetClient::OnSend()
{

}

void CNetClient::SendThread()				//发送消息线程
{

}

void RecvThread()				//接受线程 
{

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

