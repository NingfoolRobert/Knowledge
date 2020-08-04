#include "StreamIO.h"
#include "IOMgr.h"
#include "GFunc.h"
#include <string.h>

#define	SOCKET_RECV_CNT			3
#define SOCKET_RECV_PER_LENGTH		4096
	
CStreamIO::CStreamIO() :m_tBreak(0),m_pRecvBuf(nullptr), m_pSendBuf(nullptr)
{
	m_dwRecvLen = 0;
	m_dwSendLen = 0;
	m_nNewEventType = EPOLLIN;
}
	
CStreamIO::~CStreamIO()
{
}

bool CStreamIO::SendMsg(Buffer* pBuf)
{
	if(nullptr == pBuf)
		return true;

	Buffer* pTmp = new Buffer;
	if(nullptr == pBuf)
	{
		LogError("memory error.");
		return false;
	}
	
	pTmp->swap(*pBuf);

	m_clsSendLock.lock();
	m_listSendBuf.push_back(pTmp);;
	m_clsSendLock.unlock();
	PermitWrite();
	return true;
}
	
bool CStreamIO::SendMsg(PHEADER pMsg)
{
	if(nullptr == pMsg)
		return true;
	
	Buffer* pBuf = new Buffer;
	if(nullptr == pBuf)
	{
		LogError("memory error");
		return false;
	}
	
	pBuf->resize(pMsg->dwLength + sizeof(HEADER));
	
	memmove(&pBuf->at(0), (char*)pMsg, pMsg->dwLength + sizeof(HEADER));

	m_clsSendLock.lock();
	m_listSendBuf.push_back(pBuf);;
	m_clsSendLock.unlock();
	PermitWrite();
	return true;
}

bool CStreamIO::OnSend()
{
	int nSendCnt = SOCKET_RECV_CNT;
	while(nSendCnt--)
	{
		if(m_pSendBuf == nullptr)
		{
			m_clsSendLock.lock();
			if(!m_listSendBuf.empty())	
				m_pSendBuf = m_listSendBuf.front();
			m_clsSendLock.unlock();
			if(m_pSendBuf == nullptr && m_listSendBuf.size() == 0)
			{	
				ForbidWrite();
				return true;
			}
			m_dwSendLen = 0;
		}
		//
		int nSend = CSocket::Send(&m_pSendBuf->at(0) + m_dwSendLen, m_pSendBuf->size());
		if(nSend < 0)
		{
			if(EINTR == errno)
				continue;
			else if(EAGAIN == errno)
				break;
			else 
			{
				char szIP[16] = { 0 };
				HostIP2Str(m_dwIP, szIP);
				LogError("IP:Port = %s:%d, errno = %d.", szIP, m_nPort, errno);
				OnClose();
				return true;
			}
		}
		else if(nSend == 0)
		{
			char szIP[16] = { 0 };
			HostIP2Str(m_dwIP, szIP);
			LogTrace("Send  Connect break IP:Port = %s:%d", szIP, m_nPort);
			OnClose();
			return true;
		}
	
		m_dwSendLen += nSend;
		if(m_dwSendLen == m_pSendBuf->size())
		{
			delete m_pSendBuf;
			m_pSendBuf = nullptr;
		}
	}

	return true;
}
	
bool CStreamIO::OnRecv()
{
	int cnRecv = SOCKET_RECV_CNT;
	unsigned int dwDataLen = 0;
	do 
	{
		if(m_pRecvBuf == nullptr )
		{
			m_pRecvBuf = new Buffer;
			if(nullptr == m_pRecvBuf)
			{
				LogError("memory error.");
				return false;
			}
			m_pRecvBuf->clear();
			m_dwRecvLen = 0;
			dwDataLen = sizeof(HEADER);
		}
		else 
		{
			if(m_dwRecvLen >= sizeof(HEADER))
			{
				PHEADER pMsg = (PHEADER)&m_pRecvBuf->at(0);
				dwDataLen = ntohl(pMsg->dwLength) + sizeof(HEADER);
			}
			else 
				dwDataLen = sizeof(HEADER);
		}
		
		//	
		if(dwDataLen > m_pRecvBuf->size())
			m_pRecvBuf->resize(dwDataLen);
		int nRecvLen = CSocket::Recv(&(m_pRecvBuf->at(0)) + m_dwRecvLen, dwDataLen - m_dwRecvLen);
		if(nRecvLen < 0)
		{
			if(EINTR == errno)
				continue;
			else if(EAGAIN == errno)
				break;
			else 
			{
				char szIP[16] = { 0 };
				HostIP2Str(m_dwIP, szIP);
				LogError("IP:Port=%s:%d, errno= %d.", szIP, m_nPort, errno);
				OnClose();
				return true;
			}
		}
		else if(nRecvLen == 0)
		{
			char szIP[16] = { 0 };
			HostIP2Str(m_dwIP, szIP);
			LogTrace("Connection break. IP:Port=%s:%d.", szIP, m_nPort);
			OnClose();
			return true;
		}

		m_dwRecvLen += nRecvLen;			//接受到消息剩余长度
		if(m_dwRecvLen == sizeof(HEADER))
		{
			PHEADER pMsg = (PHEADER)&m_pRecvBuf->at(0);
			dwDataLen = ntohl(pMsg->dwLength) + sizeof(HEADER);
		}
		
		if(dwDataLen == m_dwRecvLen)
		{
			m_clsRecvLock.lock();
			m_listRecvBuf.push_back(m_pRecvBuf);
			m_clsRecvLock.unlock();
			m_pRecvBuf = nullptr;
			AddRef();
			if(m_pOwner) m_pOwner->AddRef();		//异步处理  
			g_pIOMgr->OnNetMsg(this);
			Release();
			m_tLast = time(NULL);
			m_dwRecvLen = 0;
			dwDataLen = 0;
		}
	}while(--cnRecv);

	return true;
}

bool CStreamIO::OnClose()
{
	if(m_pRecvBuf)
	{
		delete m_pRecvBuf;
		m_pRecvBuf = nullptr;
	}

	return CNetIO::OnClose();
}
	
void CStreamIO::PermitRead()
{
	if(!IsReadable())
		m_nNewEventType |= EPOLLIN;
}

void CStreamIO::PermitWrite()
{
	if(!IsWriteable())
		m_nNewEventType |= EPOLLOUT;
}

void CStreamIO::ForbidRead()
{
	if(IsReadable())
		m_nNewEventType = ~EPOLLIN;
}

void CStreamIO::ForbidWrite()
{
	if(IsWriteable())
		m_nNewEventType = ~EPOLLOUT;
}

bool CStreamIO::IsReadable()
{
	return m_nEventType & EPOLLIN;	
}

bool CStreamIO::IsWriteable()
{
	return m_nEventType & EPOLLOUT;
}

void CStreamIO::ProcessMsg()
{
	Buffer* pBuf = nullptr;
	m_clsRecvLock.lock();
	if(m_listRecvBuf.size())
	{
		pBuf = m_listRecvBuf.front();
		m_listRecvBuf.pop_front();
	}
	m_clsRecvLock.unlock();
	if(nullptr == pBuf)
		return ;
	
	PHEADER pMsg = (PHEADER)&pBuf->at(0);
	pMsg->dwLength = ntohl(pMsg->dwLength);	
	OnMsg(pMsg);		//Consume 

	delete pBuf;
}


