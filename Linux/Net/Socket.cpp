#include "Socket.h"
#include "Protocol.h"

#include <unistd.h>

CSocket::CSocket()
{

}
CSocket:: ~CSocket()
{

}

bool CSocket::Create(int domain/* = PF_INET*/, int nSocketType/*=SOCK_STREAM*/, int nProtocol/* = IPPROTO_TCP*/)
{
	m_fdSocket = socket(domain,nSocketType,nProtocol);
	if(m_fdSocket <= 0)
	{
		//TODO
		return false;
	}
	return true;
}

bool CSocket::Bind(unsigned int nHostPort/* = 0 */, const char* pszHostAddress/* = nullptr*/)
{
	struct sockaddr_in  Addr;
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(nHostPort);
	
	if(pszHostAddress == nullptr)
	{
		Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		Addr.sin_addr.s_addr = inet_addr(pszHostAddress);
	}

	int nRet = bind(m_fdSocket,(struct sockaddr*)&Addr,0);
	if(nRet < 0)
	{
		//TODO Log
		return false;
	}

	return true;
}

bool CSocket::Connect(const char* pszHostAddress, unsigned int nPort)
{
	struct sockaddr_in Addr;
	Addr.sin_port = htons(nPort);
	Addr.sin_addr.s_addr= inet_addr(pszHostAddress);

	socklen_t len = sizeof(Addr);
	int nRet = connect(m_fdSocket,(struct sockaddr*)&Addr,len);
	if(nRet < 0)
	{
		//TODO 
		return false;
	}
	return true;
}

//int CSocket::Recv(void* pBuf, int nBufLen, bool bRecvAll/*= false*/)
//{
//	//if((unsigned long)nBufLen < sizeof(HEADER))
//	//	return 0;
//	int nRecv = 0;
//	if(!bRecvAll)
//	{
//		nRecv =	OnRecv(pBuf,sizeof(HEADER));
//		if(nRecv == sizeof(HEADER))
//			return nRecv;
//		else 
//		{
//			while(1)
//			{
//				nRecv = OnRecv(pBuf,sizeof(HEADER));
//				if(nRecv == 0)
//					return ;
//			}
//		}
//	}	
//	if(nullptr == pMsg)
//		return 0;
//	nRecv = OnRecv((char*)pBuf + sizeof(HEADER), pMsg->uLength);
//	if(nRecv != pMsg->uLength)
//		return 0;
//
//	return sizeof(HEADER) + pMsg->uLength;
//}
int CSocket::Recv(void* pBuf, int nBufLen, bool bRecvAll /*= false*/)
{
	int nRecved = 0;
	int nRecv  = 0;
	do
	{
		nRecv = OnRecv(pBuf,nBufLen - nRecved);
		nRecved += nBufLen;
	
	}while(bRecvAll || nRecved == nBufLen);
	return nRecved;
}

int  CSocket::OnRecv(void *pBuf, int nBufLen)
{
	unsigned int  uRecved = 0;
	int nRecv = 0;
	do{
		nRecv = read(m_fdSocket, pBuf, nBufLen - uRecved);
		if(nRecv > 0)
			uRecved  += nRecv;
	}while(nRecv == -1 || errno == EAGAIN);


//	int nRecv = read(m_fdSocket,pBuf, nBufLen);
	
//
//	else
//	{
//		if(!bRecvAll  && (nRecv == sizeof(HEADER)))
//		{
//			return nRecv;
//		}
//		else
//		{
//			PHEADER pheader = (PHEADER)pBuf;
//			if(pheader == nullptr)
//	 		{
//				 //TODO
//				return 0;
//			}
//			uRecved += pheader->uLength;
//			//unsigned int uRecv=	pheader->uLength ;
//			do
//			{
//				nRecv = read(m_fdSocket,(char*)pBuf+ uRecved, pheader->uLength - uRecved);
//				uRecved +=nRecv;
//
//			}while(nRecv == -1 || nRecv == EAGAIN);
//
//		}
//	}
	return uRecved;
}

int  CSocket::Send(const void* pBuf ,int nBufLen)
{
	int nSended = 0;
	while(nSended < nBufLen)
	{		
		int nSend = write(m_fdSocket,(char*)pBuf + nSended, nBufLen - nSended);
		if(nSend < 0)
		{
			if( errno  == EAGAIN)
			{
				//对于非阻塞的socket而言，已全部发送成功
				break;
			}
			else if(errno == EINTR)
			{
			//被信号中断
			}
			//TODO  log
			return nSended;
		}
		nSended += nSend;
	}
	return nBufLen;
}

inline bool CSocket::Close()
{
	int nRet  = close(m_fdSocket);
	
	return 0 == nRet;
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
	struct sockaddr_in localaddr;
	socklen_t len = sizeof(localaddr);
	
	int nRet =  getsockname(m_fdSocket,(struct sockaddr*)&localaddr, &len);
	if(nRet < 0)
	{
		//TODO	
		return false;
	}
	
	strcpy(pszAddress,inet_ntoa(localaddr.sin_addr));
	*pSockPort = ntohs(localaddr.sin_port);
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
//	setsockopt(m_fdSocket,IPPROTO_TCP, TCP_NODELAY,&Opt,sizeof(int));
}



void CSocket::SetLinger(int nSecond)		// 设置容许数据逗留时间
{
	struct linger slinger;
//	slinger.I_onoff =1;	//closesocket() 调用后，但是还有数据没发送完毕时容许逗留 
//	slinger.I_linger = nSecond;
	
	setsockopt(m_fdSocket, SOL_SOCKET, SO_LINGER, (const char*)&slinger,sizeof(slinger));
}

void CSocket::SetKeepAlive(bool bKeepAlive)
{
	int nKeepAlive = (int)bKeepAlive;
	setsockopt(m_fdSocket,SOL_SOCKET, SO_KEEPALIVE, (char*)&nKeepAlive, sizeof(int));
}

bool CSocket::SetSockOpt(int nOptionName, const void* pOptionValue, int nOptionLen, int nLevel/* = SOL_SOCKET*/)
{
	int Ret = setsockopt(m_fdSocket, nLevel, nOptionName, pOptionValue, nOptionLen);
	return Ret == 0;
}

