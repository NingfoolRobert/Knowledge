#include "StreamIO.h"
#include "IOMgr.h"
#include "GFunc.h"
#include <string.h>

#define	SOCKET_RECV_CNT			3
#define SOCKET_RECV_PER_LENGTH		4096
	
CStreamIO::CStreamIO()
{
	m_nNewEventType = EPOLLIN;
}
	
CStreamIO::~CStreamIO()
{

}

bool CStreamIO::OnSend()
{
//	int nSendCnt = SOCKET_RECV_CNT;
//	while(nSendCnt--)
//	{
//		if(m_pSendBuf == nullptr)
//		{
//			m_clsSendLock.lock();
//			if(!m_listSendBuf.empty())	
//				m_pSendBuf = m_listSendBuf.front();
//			m_clsSendLock.unlock();
//			if(m_pSendBuf == nullptr && m_listSendBuf.size() == 0)
//			{	
//				ForbidWrite();
//				return true;
//			}
//			m_dwSendLen = 0;
//		}
//		//
//		int nSend = CSocket::Send(&m_pSendBuf->at(0) + m_dwSendLen, m_pSendBuf->size());
//		if(nSend < 0)
//		{
//			if(EINTR == errno)
//				continue;
//			else if(EAGAIN == errno)
//				break;
//			else 
//			{
//				char szIP[16] = { 0 };
//				HostIP2Str(m_dwIP, szIP);
//				LogError("IP:Port = %s:%d, errno = %d.", szIP, m_nPort, errno);
//				OnClose();
//				return true;
//			}
//		}
//		else if(nSend == 0)
//		{
//			char szIP[16] = { 0 };
//			HostIP2Str(m_dwIP, szIP);
//			LogTrace("Send  Connect break IP:Port = %s:%d", szIP, m_nPort);
//			OnClose();
//			return true;
//		}
//	
//		m_dwSendLen += nSend;
//		if(m_dwSendLen == m_pSendBuf->size())
//		{
//			delete m_pSendBuf;
//			m_pSendBuf = nullptr;
//		}
//	}

	return true;
}

	
bool CStreamIO::OnMsg(PHEADER& pMsg)
{
	if(m_pfn)
	{
		return (*m_pfn)(pMsg);
	}

	return false;
}
	
bool CStreamIO::OnRecv()
{
	int cnRecv = SOCKET_RECV_CNT;
	unsigned int dwDataLen = 0;
	do 
	{
		if(m_pRecvBuf == nullptr )
		{
			m_pRecvBuf = (char*)malloc(sizeof(HEADER));
			if(nullptr == m_pRecvBuf)
			{
				LogError("memory error.");
				return false;
			}
			m_nCapbility = sizeof(HEADER);
			m_dwRecvLen = sizeof(HEADER);
			dwDataLen = m_dwRecvLen;
		}
		else 
		{
			if(m_dwRecvLen) 
			{
				PHEADER pMsg = (PHEADER)&m_pRecvBuf;
				dwDataLen = pMsg->dwLength + sizeof(HEADER);
			}
			else
				dwDataLen = sizeof(HEADER);
			
		}
		//	
		if(dwDataLen > m_nCapbility)
		{
			char* pBuf = (char*)realloc(m_pRecvBuf, dwDataLen);
			if(nullptr == pBuf)
			{
				LogError("memory error");
				return false;
			}
			m_pRecvBuf = pBuf;
		}

		int nRecvLen = CSocket::Recv(m_pRecvBuf + m_dwRecvLen, dwDataLen - m_dwRecvLen);
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
//
		if(dwDataLen == sizeof(HEADER))
		{
			PHEADER pMsg = (PHEADER)m_pRecvBuf;
			m_dwRecvLen = pMsg->dwLength + sizeof(HEADER);
		}
		
		m_dwRecvLen += nRecvLen;			//
		if(m_dwRecvLen == dwDataLen)
		{
			PHEADER pMsg = (PHEADER)m_pbuf;
			OnMsg(pMsg);
			//
			if(nullptr == pMsg)
				m_nCapbility = 0;
			m_dwRecvLen = 0;
			break;
		}
	}while(--cnRecv);

	return true;
}

bool CStreamIO::OnClose()
{
	m_nNewEventType = EPOLL_EVENT_TYPE_CLOSE;
	m_tBreak = time(NULL);
	if(m_pRecvBuf)
	{
		free(m_pRecvBuf);
		m_pRecvBuf = nullptr;
		m_nCapbility = 0;
		m_dwRecvLen = 0;
	}

	return true;
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
	
	OnMsg(pMsg);		//Consume 
	
	delete pBuf;
}

///////////////////////////////////////////////////////////////
CAcceptIO::CAcceptIO():m_bStop(false)
{
	m_nNewEventType = EPOLLIN ;
}
	
CAcceptIO:: ~CAcceptIO()
{
	
}
	
bool CAcceptIO::IsReadable()
{
	if(m_nEventType & EPOLLIN)
		return true;
	
	return false;
}
	
bool CAcceptIO::OnInit(int nPort)
{
	int fd = Create();
	if(fd < 0)
	{
		LogError("Create Socket fail.");
		return false;
	}

	int nSwitch = 1;
	CSocket::SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &nSwitch, sizeof(nSwitch));
	
	if(!Bind(nPort))
	{
		return false;
	}
	
	if(!Listen())
	{
		return false;
	}

	return true;
}

bool CAcceptIO::OnRecv()
{
	while(IsReadable())
	{
		int fd = CSocket::Accept();
		if(fd <= 0)
		{
			if(EINTR == errno)
				continue;
			else 
			{
				if(EMFILE == errno)
					LogFatal("the file handle is exhaust.");
				
				break;
		
			}
		}

		CStreamIO* pNetClient = new CStreamIO;
		if(nullptr == pNetClient)
		{
			LogError("memory error.");
			return false;
		}
		pNetClient->Attach(fd);
		struct sockaddr_in sa;
		socklen_t nlen = sizeof(sa);
		pNetClient->GetPeerName((struct sockaddr*)&sa, &nlen);
		pNetClient->m_dwIP = ntohl(sa.sin_addr.s_addr);
		pNetClient->m_nPort = ntohs(sa.sin_port);
		pNetClient->AddRef();
		g_pIOMgr->AddNetIO(pNetClient);
	}

	return true;
}

bool CAcceptIO::OnClose()
{	
	ForbidRead();
	OnClose();
	return true;
}

///////////////////////////////////////////////////////	
CActiveIO::CActiveIO()
{
	m_nConnectStatus = CONNECT_STAT_TYPE_UNINIT;
}

CActiveIO::~CActiveIO()
{
	if(m_nConnectStatus < CONNECT_STAT_TYPE_CLOSING)
	{
		CSocket::Close();
		m_nConnectStatus = CONNECT_STAT_TYPE_CLOSED;
	}
}

bool CActiveIO::OnInit(const char* pszHostIP, int nPort)
{
	if(nullptr == pszHostIP)
	{
		LogError("IP invalid. ");
		return false;
	}
	
	unsigned int dwIP = Str2HostIP(pszHostIP);
	return OnInit(dwIP, nPort);
}

bool CActiveIO::OnInit(const unsigned int dwHostIP, int nPort)
{
	m_dwIP = dwHostIP; 
	m_nPort = nPort;
	int fd  = CSocket::Create();
	if(fd < 0)
	{
		LogError("Create socket fail. error: %d", errno);
		return false;
	}

	//TCP_NODELAY 
	bool bRet = CSocket::Connect(dwHostIP, nPort);
	if(!bRet)
	{
		char szIP[16] = { 0 };
		LogError("Connect fail. IP:Port=%s:%d. errno = %d", HostIP2Str(m_dwIP, szIP), m_nPort, errno );		
		return false; 
	}

	CSocket::SetBlockMode();
	PermitWrite();
	PermitRead();
	g_pIOMgr->AddNetIO(this);	
	return bRet;
}

bool CActiveIO::ReConnect()
{
	char szIP[16] = { 0 };
	LogTrace("Reconnect Dst IP:Port = %s:%d", HostIP2Str(m_dwIP, szIP), m_nPort);
	return OnInit(m_dwIP, m_nPort);
}
	
bool CActiveIO::OnClose()
{
	CStreamIO::OnClose();

	OnBreak();
	m_nConnectStatus = CONNECT_STAT_TYPE_CLOSING;
	return true;
}
	
void CActiveIO::OnBreak()
{
	LogInfo("");
	return true;
}
	
void CActiveIO::Terminate()
{
	OnClose();
	return true;
}


