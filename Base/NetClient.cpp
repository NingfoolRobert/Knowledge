#include "NetClient.h"
#include "UserObject.h"
#include "GFunc.h"
#include "NetService.h"
#include "ThreadPool.h"
#include <functional>




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


bool CNetClient::OnInitialUpdate(CNetService* pNetService,int fd)		//初始化Socket, UserObject, 
{	
	if(pNetService == nullptr || fd <= 0)
	{
		LogError("%s(%d) NetService is NULL or fd <= 0.", __FILE__, __LINE__);
		return false;
	}
	Attach(fd);

//	char szAddress[32] = { 0 };

	struct sockaddr_in PeerAddr;
	socklen_t socklen = sizeof(PeerAddr);
	GetPeerName((struct sockaddr*)&PeerAddr, &socklen);

	GetDWORDNetIPAndPort(m_dwIP, m_nPort, (struct sockaddr*)&PeerAddr);


	return true;
}

bool CNetClient::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}
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
	auto task  = std::bind(&CNetClient::RecvThread, this);
	PostEvent(task);
}

void CNetClient::OnSend()
{
	// task  add in threadpool
	auto task  = std::bind(&CNetClient::SendThread, this);

	PostEvent(task);
}

void CNetClient::SendThread()				//发送消息线程
{
	
	char* pSendHead = m_pbufSend->GetBufPtr() + m_dwSendOver;
	
	m_dwSendOver += CNetIO::Send(pSendHead, m_dwSendLeft);
	if(errno == EAGAIN )
	{
		if(m_dwSendOver < m_dwSendBlockSize)
		{
			CNetIO::UpdateEventType(EPOLLOUT | EPOLLET);
		}
	}
	if(m_dwSendOver == m_dwSendBlockSize)
	{
		m_pbufSend  = m_listSendMsg.front();
		
	}
}

void CNetClient::RecvThread()				//接受线程 
{


	PHEADER pMsg = nullptr;
	m_pNetSercie->OnNetNetMsg(this, pMsg);
}

bool CNetClient::OnMsg(PHEADER pMsg)		//消息到达网络端口
{
	if(nullptr == pMsg || nullptr == m_pUserObj)
	{
		return false;
	}

	return m_pUserObj->OnMsg(pMsg);

}

bool CNetClient::OnConnect()				//链接消息网络接口
{
	if(nullptr == m_pUserObj)
	{
		return false;
	}
	if(m_dwRecvSerial == 0)
	{
		m_pUserObj->OnConnect();	
	}
	
	return true;
}

bool CNetClient::OnBreak()					//网络断开
{
	return true;
}

inline bool CNetClient::IsBindUserObject()
{
	if(nullptr == m_pUserObj)
		return false;
	return true;
}
