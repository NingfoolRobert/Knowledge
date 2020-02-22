#include "Connection.h"
#include "AutoLock.h"
#include "LogFile.h"
#include "GFunc.h"

#define RECV_MSG_GAP_COUNT					400
#define CONNECTIONPOOL_DEFAULT_COUNT		3

//TODO 断线重连机制 Send 数据模式

CConnection::CConnection():m_pHeader(nullptr), m_pBuf(nullptr),m_pAddrInfo(nullptr)
{
	m_pBuf = new CBuffer;
}

CConnection:: ~CConnection()
{
	if(m_pHeader)
	{
		m_pHeader = nullptr;
	}
	if(m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = nullptr;
	}
	if(m_pAddrInfo)
	{
		delete m_pAddrInfo;
		m_pAddrInfo = nullptr;
	}
}


bool CConnection::InitialUpdate(unsigned int dwHostIP, int nPort)
{
	m_pAddrInfo = new ADDRINFO;
	if(nullptr == m_pAddrInfo)
	{
		return false;
	}
	m_pAddrInfo->dwHostIP = dwHostIP;
	m_pAddrInfo->nPort = nPort;
	m_pAddrInfo->dwNetIP = htonl(dwHostIP);
	HostIP2Str(dwHostIP, m_pAddrInfo->szIP);
	
	if(CSocket::Create() <= 0)
	{
		return false;
	}
	//
	if(!CSocket::Connect(dwHostIP, nPort) )
	{
		char szIP[32] = { 0 };
		HostIP2Str(dwHostIP, szIP);
		LogError("Connect Server fail.IP:Port=%s:%d, errno:%d", szIP, nPort, errno);
		return false;
	}
	//Active 	
	int nKeepAlive = 1;
	int nKeepIdle = 60;
	int nKeepInterval = 5;
	int nKeepCount = 3;
	
	CSocket::SetSockOpt(SOL_SOCKET, SO_KEEPALIVE, (void*)&nKeepAlive, sizeof(nKeepAlive));
	CSocket::SetSockOpt(SOL_TCP, TCP_KEEPIDLE, (void*)&nKeepIdle,sizeof(nKeepIdle));
	CSocket::SetSockOpt(SOL_TCP, TCP_KEEPINTVL, (void*)&nKeepAlive, sizeof(nKeepInterval));
	CSocket::SetSockOpt(SOL_TCP, TCP_KEEPCNT, (void*)&nKeepCount, sizeof(nKeepCount));

	return true;
}

bool CConnection::InitialUpdate(const char* pszIP, int nPort)
{	
	unsigned int dwHostIP  = String2HostIP(pszIP);
	
	return InitialUpdate(dwHostIP, nPort);
}
	
bool CConnection::CheckConnectState(bool Reconnect /*= true*/)
{
	//TODO 断线重连
	
	
	return true;
}

bool CConnection::SendMsg(PHEADER pHeader)
{
	if(nullptr == pHeader)
	{
		return false;
	}
	
	return CSocket::Send((char*)pHeader, pHeader->dwLength + sizeof(HEADER))  == (int)sizeof(HEADER) + (int)pHeader->dwLength;
}
//	
bool CConnection::SendMsg(unsigned int dwHostIP, int nPort, PHEADER pHeader)
{
	if(!InitialUpdate(dwHostIP, nPort))	
	{
		return false;
	}
	
	return SendMsg(pHeader);
}
	
PHEADER CConnection::PeerMsg()
{
	m_pHeader = nullptr;
	CSocket::SetBlockMode();
	struct timeval tv;
	fd_set fdReadSet;
	
	int nRet = 0;
	char szBuf[1024] = { 0 };
	
	for(int i = 0; i < RECV_MSG_GAP_COUNT; ++i)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 5 * 1000;
		FD_ZERO(&fdReadSet);
		FD_SET(m_fd, &fdReadSet);
		nRet = select(m_fd + 1, &fdReadSet, nullptr, nullptr, &tv);
		if(nRet == 0)
			continue;
		else if(nRet < 0)
		{
			LogError("%s(%d) select fail.", __FILE__, __LINE__);
			break;
		}
		//
		if(FD_ISSET(m_fd, &fdReadSet))
		{
			int nLen = 0;
			while(true)
			{
				nLen = CSocket::Recv(szBuf, 1024);
				if(nLen > 0)
				{
					if(!m_pBuf->Append(szBuf, nLen))
					{
						LogError("%s(%d) Apend Data fail.", __FILE__, __LINE__);
						CSocket::SetBlockMode(~O_NONBLOCK);
						return nullptr;
					}

				}
				else if(nLen == 0)
				{
					LogError("%s(%d) Connection closed.", __FILE__, __LINE__);
					return nullptr;
				}
				else 
				{
					if(EAGAIN == errno)
						break;
					else if(EINTR == errno)
						continue;
					else 
					{
						LogError("%s(%d) Recv Data errno:%d.", __FILE__, __LINE__, errno);
						return nullptr;
					}
				}
			}
		}
		
		m_pHeader = (PHEADER)m_pBuf->GetBufPtr();
		if(m_pHeader->dwLength + (unsigned int)sizeof(HEADER) <= (unsigned int)m_pBuf->GetBufLen())
		{
			break;
		}
	}

	CSocket::SetBlockMode(~O_NONBLOCK);
	return m_pHeader;
}

bool CConnection::SendMsg(PHEADER pHeader, bool bAck, int nOrgine, unsigned int  dwAckType, CBuffer* pBuffer)
{
	if(nullptr == pHeader)
	{
		return false;
	}
	
	int nLen = CSocket::Send((char*)pHeader, pHeader->dwLength + sizeof(HEADER));
	if(nLen <= 0)
	{
		LogError("%s(%d) Connection fail. IP:Port=%s:%d", __FILE__ ,__LINE__,m_pAddrInfo->szIP, m_pAddrInfo->nPort);
		return false;
	}
	
	if(!bAck)
		return true;
	//
	fd_set ReadSet;
	struct timeval tv;
	
	CSocket::SetBlockMode();
	int nRet = 0;
	char szBuf[1024] = { 0 };
	int nOffset = 0;
	for(int i = 0; i < RECV_MSG_GAP_COUNT; ++i)
	{
		FD_ZERO(&ReadSet);
		FD_SET(m_fd, &ReadSet);
		tv.tv_sec = 0;
		tv.tv_usec = 5 * 1000;
		nRet = select(m_fd + 1, &ReadSet, nullptr, nullptr, &tv);
		if(nRet == 0)
			continue;
		else if(nRet < 0)
		{
			LogError("Recv Data fail. IP:Port = %s:%d, errno=%d", m_pAddrInfo->szIP, m_pAddrInfo->nPort, errno);
			continue;
		}
		//
		if(FD_ISSET(m_fd, &ReadSet))
		{
			while(true)
			{
				int nLen = CSocket::Recv(szBuf, 1024);
				if(nLen > 0)
				{
					if(!m_pBuf->Append(szBuf, nLen))
					{
						LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
						CSocket::SetBlockMode(~O_NONBLOCK);
						return false;
					}
				}
				else if(nLen < 0)
				{
					if(EAGAIN == errno)
					{
						break;
					}
					else if(EINTR == errno)
					{
						continue;
					}
					else 
					{
						LogError("%s(%d) Select: Recv Message error.errno= %d", __FILE__, __LINE__, errno);
						CSocket::SetBlockMode(~O_NONBLOCK);
						return false;
					}
				}
				else if(nLen == 0)
				{
					LogError("%s(%d) Connnection closed. IP:Port=%s:%d", __FILE__, __LINE__, m_pAddrInfo->szIP, m_pAddrInfo->nPort);
					CSocket::SetBlockMode(~O_NONBLOCK);
					return false;
				}
			}
			//
			m_pHeader = (PHEADER)(m_pBuf->GetBufPtr() + nOffset);
			//
			while(m_pHeader->wOrigin != nOrgine || m_pHeader->dwType != dwAckType)
			{
				if(nOffset < m_pBuf->GetBufLen() - HEADSIZE)
					nOffset++;
				else 
					break;
				m_pHeader = (PHEADER) (m_pBuf->GetBufPtr() + nOffset);
			}
			//	
			if(m_pHeader->wOrigin == nOrgine && m_pHeader->dwType == dwAckType)
			{
				if(m_pHeader->dwLength + (unsigned int)sizeof(HEADER) <=(unsigned int) m_pBuf->GetBufLen() - (unsigned int)nOffset)
				{
					if(!pBuffer->Append(m_pHeader, m_pHeader->dwLength + sizeof(HEADER)))
					{
						LogError("%s(%d) Apend Data fail.", __FILE__, __LINE__);
						CSocket::SetBlockMode(~O_NONBLOCK);
						return false;
					}
					break;
				}
			}
		}
	}
	//
	CSocket::SetBlockMode(~O_NONBLOCK);
	m_pHeader = nullptr;
	m_pBuf->Clear(true);
	return false;
}



/////////////////////////////////////////////////////////////////
CConnectionPool::CConnectionPool():m_nConnMax(CONNECTIONPOOL_DEFAULT_COUNT),m_bClose(false)
{
	m_pAddrInfo = new ADDRINFO;
	memset(m_pAddrInfo, 0, sizeof(ADDRINFO));
}
//
CConnectionPool::~CConnectionPool()
{
}
//
CConnection* CConnectionPool::GetConnection()
{
	CConnection* pConn = nullptr;
	m_clsLock.Lock();
	if(!m_listConnection.empty())
	{
		pConn = m_listConnection.front();
		m_listConnection.pop();
		m_clsLock.UnLock();
		struct tcp_info info;
		int nLen = sizeof(info);
		pConn->GetSockOpt(IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&nLen);
		if(info.tcpi_state == TCP_ESTABLISHED)
		{
			return pConn;
		}
		else 
			delete pConn;
	}
	
	pConn = new CConnection;
	if(nullptr == pConn)
	{
		LogError("%s(%d) memory error.", __FILE__, __LINE__);
		return pConn;
	}
	//	
	if(!pConn->InitialUpdate(m_pAddrInfo->dwHostIP, m_pAddrInfo->nPort))
	{
		delete pConn;
		return nullptr;
	}

	return pConn;
}
	
void CConnectionPool::ReclaimConn(CConnection* pConn)
{
	if(nullptr == pConn)
	{
		return ;
	}
	//
	pConn->SetBlockMode(~O_NONBLOCK);
	//
	CAutoLock locker(&m_clsLock);
	if((int)m_listConnection.size() >= m_nConnMax || m_bClose)
	{
		delete pConn;
		pConn = nullptr;
	}
	else 
	{
		m_listConnection.push(pConn);
	}
}

bool CConnectionPool::InitialUpdate(const char* pszIP, int nPort, int nConnCount/* = 3*/)
{	
	if(nullptr == pszIP)
	{
		return false;
	}
	//
	if(nullptr == m_pAddrInfo)
	{
		m_pAddrInfo = new ADDRINFO;
		if(nullptr == m_pAddrInfo)
		{
			LogError("%s(%d) memmory error.");
			return false;
		}
	}
	
	strcpy(m_pAddrInfo->szIP, pszIP);
	m_pAddrInfo->dwNetIP = inet_addr(pszIP);
	m_pAddrInfo->dwHostIP = ntohl(m_pAddrInfo->dwNetIP);
	m_pAddrInfo->nPort = nPort;
	
	m_nConnMax = nConnCount;
	return true;
}

bool CConnectionPool::SetAddrInfo(const char* pszIP, int nPort)
{
	if(nullptr == pszIP)
	{
		return false;
	}
	if(nullptr == m_pAddrInfo)
	{
		m_pAddrInfo = new ADDRINFO;
		if(nullptr == m_pAddrInfo)
		{
			LogError("%s(%d) memmory error.");
			return false;
		}
	}
	
	strcpy(m_pAddrInfo->szIP, pszIP);
	m_pAddrInfo->dwNetIP = inet_addr(pszIP);
	m_pAddrInfo->dwHostIP = ntohl(m_pAddrInfo->dwNetIP);
	m_pAddrInfo->nPort = nPort;
	
	return true;
}

bool CConnectionPool::SetAddrInfo(PADDRINFO* pAddrInfo)
{
	if(nullptr == m_pAddrInfo)
	{
		m_pAddrInfo = new ADDRINFO;
		if(nullptr == m_pAddrInfo)
		{
			LogError("%s(%d) memmory error.");
			return false;
		}
	}

	memcpy(m_pAddrInfo, pAddrInfo, sizeof(ADDRINFO));
	return true;
}

bool CConnectionPool::SetAddrInfo(unsigned int dwHostIP, int nPort)
{
	if(nullptr == m_pAddrInfo)
	{
		m_pAddrInfo = new ADDRINFO;
		if(nullptr == m_pAddrInfo)
		{
			LogError("%s(%d) memmory error.");
			return false;
		}
	}
	m_pAddrInfo->dwHostIP = dwHostIP;
	m_pAddrInfo->nPort = nPort;
	m_pAddrInfo->dwNetIP = htonl(dwHostIP);
	HostIP2Str(dwHostIP, m_pAddrInfo->szIP);
	return false;
}

bool CConnectionPool::Terminate()
{
	return false;
}
