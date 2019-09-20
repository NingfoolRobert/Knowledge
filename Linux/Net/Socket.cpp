#include "Socket.h"
#include "Protocol.h"

#include <unistd.h>

CSocket::CSocket()
{

}
CSocket:: ~CSocket()
{

}

bool CSocket::Create(unsigned int nPort /*= 0*/, const char* pszAddress /*= nullptr*/, int nSocketType/* = SOCK_STREAM*/, int nProtocol /*= AF_INET*/)
{
	m_addr.sin_family	= nProtocol;
	m_addr.sin_port		= htons(nPort);
	if(pszAddress == nullptr)
	{
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		m_addr.sin_addr.s_addr = inet_addr(pszAddress);
	}
	m_fdSocket = socket(nPortocol,nSocketType,0);
	if(m_fdSocket <= 0)
	{
		//TODO
		return false;
	}
	return true;
}

bool CSocket::Bind(unsigned int nHostPort/* = 0 */, const char* pszHostAddress/* = nullptr*/)
{
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(nHostPort);
	
	if(pszHostAddress == nullptr)
	{
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		m_addr.sin_addr.s_addr = inet_addr(pszHostAddress);
	}

	int nRet = bind(m_fdSocket,(struct sockaddr*)&m_addr,0);
	if(nRet < 0)
	{
		//TODO Log
		return false;
	}

	return true;
}

bool CSocket::Connect(const char* pszHostAddress, unsigned int nPort)
{
	m_addr.sin_port = htons(nPort);
	m_addr.sin_addr.s_addr= inet_addr(pszHostAddress);

	socklen_t len = 0;
	int nRet = connect(m_fdSocket,(struct sockaddr*)&m_addr,&len);
	if(nRet < 0)
	{
		//TODO 
		return false;
	}
	return true;
}

int  CSocket::Recv(void *pBuf, int nBufLen,int nFlags/* = 0*/, bool bRecvAll/* = false*/)
{
	
	int nRecv = read(m_fdSocket,pBuf,sizeof(HEADER));
	if(nRecv < 0)
	{

	}
	else if(nRecv == 0)
	{

	}
	else
	{
		if(!bRecvAll  && (nRecv == sizeof(HEADER)))
		{
			return nRecv;
		}
		else
		{
			unsigned int  uRecved = 0;
			PHEADER pheader = (PHEADER)pBuf;
			if(pheader == nullptr)
	 		{
				 //TODO
				return 0;
			}
			uRecved += pheader->uLength;
			unsigned int uRecv=	pheader->uLength ;
			do
			{
				nRecv = read(m_fdSocket,pBuf+ uRecved,nBufLen - uRecved);

				if(nRecv == -1)
				{

				}
				else if(nRecv == EAGIN)
				{
					//TODO 链接已断开
					
				} 
			}while(nRecv == -1 || nRecv == EAGIN);

		}
	}
	return 0;
}

int  CSocket::Send(const void* pBuf ,int nBufLen, int nFlags/* = 0*/)
{
	int nSended = 0;
	while(nSended < nBufLen)
	{		
		int nSend = write(m_fdSocket, pBuf + nSended, nBufLen - nSended);
		if(nSend < 0)
		{
			//TODO  log
			return nSended;
		}
		nSended += nSend;
	}
	return nBufLen;
}

inline bool CSocket::Close()
{
	close(m_fdSocket);
	return 0;
}

inline bool CSocket::Attach(int fd_socket)
{
	m_fdSocket = fd_socket;
	return true;
}

inline int  CSocket::Detach()
{
	return m_fdSocket;
}

bool CSocket::GetPeerName(char* pszAddress,int *pPeerPort)		//获取连接对端地址
{
	struct sockaddr_in PeerAddr;
	memset(&PeerAddr,0,sizeof(PeerAddr));
	socklen_t len = sizeof(PeerAddr);
	int nRet = getpeername(m_fdSocket,(struct sockaddr*)&PeerAddr,&len);
	if(nRet < 0)
	{
		//TODO  
		return false;
	}
	strcpy(pszAddress,inet_ntoa(PeerAddr.sin_addr));
	
	*pPeerPort = ntohs(PeerAddr.sin_port);

	return true;
}

bool CSocket::GetSockName(char* pszAddress,int *pSockPort)		//获取监听的地址及端口
{
	struct sockaddr localaddr;
	socklen_t len = sizeof(localaddr);
	
	int nRet =  getsockname(m_fdSocket,(struct sockaddr*)&m_addr,&len);
	if(nRet < 0)
	{
		//TODO	
		return false;
	}
	
	strcpy(pszAddress,inet_ntoa(m_addr.sin_addr));
	*pSockPort = ntohs(m_addr.sin_port);
	return true;
}

inline void CSocket::SetNonBlocking()
{	
	fcntl(m_fdSocket,F_SETFL,fcntl(m_fdSocket,F_GETFL,0) | O_NONBLOCK);
}

void CSocket::SetTimeOut(int nSecond)
{
	struct timeval set_time;
	set_time.tv_sec = nSecond;
	set_time.tv_usec = 0;
	setsockopt(m_fdSocket, SOL_SOCKET,SO_SNDTIMEO,&set_time,sizeof(struct timeval));
	setsockopt(m_fdSocket, SOL_SOCKET,SO_RCVTIMEO,&set_time,sizeof(struct timeval));
}


void CSocket:: SetNoDelay(bool bNoDelay)
{
	int Opt = 0;
	if(bNoDelay)
		Opt = 1;
	setsockopt(m_fdSocket,IPPROTO_TCP, TCP_NODELAY,&Opt,sizeof(int));
}



void CSocket::SetLinger(int nSecond)		// 设置容许数据逗留时间
{
	struct linger slinger;
	slinger.I_onoff =1;	//closesocket() 调用后，但是还有数据没发送完毕时容许逗留 
	slinger.I_linger = nSecond;
	
	setsockopt(m_fdSocket, SOL_SOCKET, SO_LINGER, (const char*)&slinger,sizeof(slinger));
}

void CSocket::SetKeepAlive(bool bKeepAlive)
{
	int nKeepAlive = (int)bKeepAlive;
	setsockopt(m_fdSocket,SOL_SOCKET, SO_KEEPALIVE, (char*)&nKeepAlive, sizeof(int));
}


