#include "NetClient.h"
#include "GFunc.h"
#include "NetService.h"
#include "LogFile.h"

#define  SOCKET_RECV_MAX_CNT	3
#define  SOCKET_RECV_LENGTH		1024

#define SOCKET_BUFFER_LEN 2048

CNetClient::CNetClient():m_pRecvBuf(nullptr),  m_dwRecvSerialNum(0),m_dwRecvLen(0), m_pSendBuf(nullptr), m_dwSendSerialNum(0), m_dwSendLen(0), m_pUserObject(nullptr)
{
	
}
	
CNetClient::~CNetClient()
{

}

bool CNetClient::OnRecv()
{
	if(nullptr == m_pRecvBuf)
	{
		m_pRecvBuf = g_pBufferMgr->GetBuffer(SOCKET_BUFFER_LEN);
		if(nullptr == m_pRecvBuf)
		{
			LogError("memory error.");
			return false;
		}
	}
	//
	char szBuf[SOCKET_BUFFER_LEN] = { 0 };
	int nRecv = 0;
	int nMaxRecvCnt = SOCKET_RECV_MAX_CNT;
	
	do 
	{
		int cnLeft = m_pRecvBuf->GetCapbility() - m_pRecvBuf->GetBufLen();
		if(cnLeft < SOCKET_RECV_LENGTH)
			m_pRecvBuf->Expand(SOCKET_RECV_LENGTH);

		nRecv = CSocket::Recv(m_pRecvBuf->GetBufPtr() + m_pRecvBuf->GetBufLen(),  SOCKET_RECV_LENGTH);
		if(nRecv < 0)
		{
			if(errno == EINTR)
				continue;			//进程调试 子进程退出 
			else if(EAGAIN  == errno)
				break;				//缓冲区无数据可读 
			else 
				OnClose();
		}
		else if(nRecv  == 0)
		{
			OnClose();
		}
	
		m_pRecvBuf->AddDataLen(nRecv);
		nRecv = 0;
	}while(--nMaxRecvCnt >= 0);

	//TODO 

	if(m_pRecvBuf->GetBufLen()  < sizeof(HEADER))
		return true;

	PHEADER pHeader  = (PHEADER)m_pRecvBuf->GetBufPtr();
	//TODO Check 
	//Parse Msg
	int nMsgLen = m_pRecvBuf->GetBufLen();
	while((unsigned int)nMsgLen >= sizeof(HEADER) + pHeader->dwLength)
	{
		CBuffer* pBuffer = g_pBufferMgr->GetBuffer(sizeof(HEADER) + pHeader->dwLength);
		if(nullptr == pBuffer)
		{
			LogError("Append data fail.");
			return false;
		}

		//Add Buffer into Recvlist;
		m_clsRecvLock.Lock();
		m_listRecvBuffer.Add(pBuffer);
		m_clsRecvLock.UnLock();
		//Add Work NetClient 
		if(!g_pNetService->OnNetMsg(this, pHeader))
		{
			OnClose();
			return false; 
		}
		nMsgLen -= sizeof(HEADER) + pHeader->dwLength;
		if(nMsgLen == 0) break;
		pHeader = (PHEADER)(m_pRecvBuf->GetBufPtr() + sizeof(HEADER) + pHeader->dwLength);
	}
	//Update m_pRecvBuf 
	if(nMsgLen != m_pRecvBuf->GetBufLen())
	{
		char* pTmp = m_pRecvBuf->GetBuffer();
		char* pData =  pTmp + m_pRecvBuf->GetBufLen() - nMsgLen;
		memmove(pTmp, pData, nMsgLen);
		m_pRecvBuf->AddDataLen(nMsgLen - m_pRecvBuf->GetBufLen());
	}

	return true;
}

bool CNetClient::OnSend()
{
	CAutoLock locker(&m_clsSendLock);
	if(!IsWriteable())	
	{
		if(!m_listSendBuffer.empty())
			PermitWrite();
		return true;
	}

	while(!m_listSendBuffer.empty()) 
	{
		if(m_pRecvBuf == nullptr || m_dwSendLen >= m_pSendBuf->GetBufLen())
		{
			g_pBufferMgr->ReleaseBuffer(m_pRecvBuf);
			m_pRecvBuf = nullptr; 

			m_pRecvBuf = m_listSendBuffer.Get();
			if(nullptr == m_pRecvBuf)
				break;
			m_dwSendLen = 0;
		}

		//Send 
		if(m_pSendBuf == nullptr) break;
		
		int nSendLen = Write(m_pSendBuf->GetBuffer() + m_dwSendLen, m_pSendBuf->GetBufLen() - m_dwSendLen);
		if(nSendLen < 0)
		{
			if(errno == EINTR)
				continue;
			else if(errno == EAGAIN)
				break;
			else 
				OnClose();
		}
		else if(nSendLen == 0)
			 OnClose();			//对端断开
		else 
			m_dwSendLen += nSendLen;
	}
	return true;
}

bool CNetClient::OnClose()
{
	shutdown(m_fd, SHUT_RDWR);
	m_tBreak = time(NULL);
	m_nNewEventType = EPOLL_EVENT_TYPE_CLOSE;
	return true;
}
	
void CNetClient::ProcessMsg()
{
	CBuffer* pBuf = nullptr;
	while(!m_listRecvBuffer.empty())
	{
		m_clsRecvLock.Lock();
		pBuf = m_listRecvBuffer.Get();
		m_clsRecvLock.UnLock();
		if(nullptr == pBuf)
			break;
	
		PHEADER pMsg = (PHEADER)pBuf->GetBufPtr();		
		m_pUserObject->OnMsg(pMsg);
		g_pBufferMgr->ReleaseBuffer(pBuf);
		pBuf = nullptr;
	}
}
	
bool CNetClient::SendMsg(PHEADER pMsg)
{
	if(nullptr == pMsg)
		return false;

	if(!IsWriteable())
	{
		
	}

	return true;
}
	
void CNetClient::BindUserObject(CUserObject* pUserObject)
{
	m_pUserObject = pUserObject;
}

bool CNetClient::IsBindUserObject()
{
	return m_pUserObject != nullptr;
}
	
CUserObject* CNetClient::GetUserObject()
{
	return m_pUserObject;
}
