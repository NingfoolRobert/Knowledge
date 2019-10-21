#include "Socket.h"
#include <memory>
#include <string.h>

CSocket::CSocket()
{
	m_fd = 0;
}
CSocket::~CSocket()
{
	Close();
}

int  CSocket::Create(int nDomain/* = AF_INET*/, int nSockType/* = SOCK_STREAM*/, int nProtocol/* = IPPROTO_IP*/)
{
	m_fd = socket(nDomain, nSockType, nProtocol);
	if(m_fd  < 0)
	{
		return 0;
	}

	return m_fd;
}

bool CSocket::Bind(int nHostPort/* = 0*/, const char* pszHostAddress/* = nullptr*/)
{
	struct sockaddr_in SvrAddr;
	memset(&SvrAddr, 0, sizeof(SvrAddr));
	
	SvrAddr.sin_family = AF_INET;
	//
	if(pszHostAddress == nullptr)
		SvrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else 
		SvrAddr.sin_addr.s_addr = inet_addr(pszHostAddress);
	//
	SvrAddr.sin_port = htons(nHostPort);

	if(bind(m_fd, (struct sockaddr*)&SvrAddr,sizeof(SvrAddr)) == -1)
	{
		return false;
	}

	return true;
}

bool CSocket::Listen(int nListenCount/* = 10*/)
{
	if(listen(m_fd, nListenCount) == -1)
	{
		return false;
	}
	return true;
}

int CSocket::Accept()
{
	struct sockaddr_in  ClientAddr;
	memset(&ClientAddr, 0, sizeof(ClientAddr));

	socklen_t sockLen = sizeof(ClientAddr);

	int fdClient = accept(m_fd, (struct sockaddr*)&ClientAddr,&sockLen);
	if(fdClient == -1)
	{
		return 0;
	}
	return fdClient;
}
bool CSocket::Connect(const char* pszAddress, int nPort)
{
	if(pszAddress == nullptr)
		return false;
	
	struct sockaddr_in SvrAddr;
	memset(&SvrAddr, 0, sizeof(SvrAddr));

	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = inet_addr(pszAddress);
	SvrAddr.sin_port = htons(nPort);

	int nRet = 0;
	if((nRet = connect(m_fd, (struct sockaddr*)&SvrAddr,sizeof(SvrAddr)) ) < 0)
	{
		return false;
	}
	return true;
}

int CSocket::Recv(char* pszBuf, int nBufLen)
{
	return read(m_fd, pszBuf, nBufLen);

	//``	int nRecved = 0;
//``	int nRead = 0;
//``	while(true)
//``	{
//``		nRead= read(m_fd, pszBuf + nRecved, nBufLen - nRecved);
//``		if(nRead == 0)
//``			break;
//``		else if(nRead < 0)
//``		{
//``			if(errno == EINTR)
//``				continue;
//``			else 
//``				return false;
//``		}
//``		
//``		nRecved += nRead;
//``		if(nRecved >= nBufLen)
//``			break;
//``	}
//``	return true;
}

bool CSocket::Send(const char* pszBuf, int nBufLen)
{
	int nSended = 0;
	do 
	{
		int nSend = write(m_fd, pszBuf + nSended, nBufLen - nSended);
		if(nSend < 0 )
		{
			if(errno == EINTR)
				continue;
			else 
			{
				//TODO Log
				return false;
			}
		}

		nSended += nSend;
	}while(nSended < nBufLen);

	
	return true;
}

void CSocket::Close()
{
	int nRet = close(m_fd);
	if( nRet )
	{
		//TODO 
		;
	}
}

bool CSocket::Attach(int nfd)
{
	if(!nfd)
	{
		return false;
	}
	m_fd = nfd;
	return true;
}

int  CSocket::Detach()
{
	return m_fd;
}
bool CSocket::GetSockName(struct sockaddr* LocalSockaddr, socklen_t* nAddrLen)
{
	if(nullptr == LocalSockaddr || nullptr == nAddrLen)
		return false;
	int nRet = getsockname(m_fd, LocalSockaddr, nAddrLen);

	return 0 == nRet;
}

bool CSocket::GetPeerName(struct sockaddr* PeerSockAddr, socklen_t* nAddrLen)
{
	if(nullptr == PeerSockAddr || nullptr == nAddrLen)
		return false;
	int nRet = getpeername(m_fd, PeerSockAddr, nAddrLen);

	return 0 == nRet;
}

bool CSocket::SetSockOpt(int nLevel, int nOptanme, const void* Optval, socklen_t OptLen)
{
	int nRet = setsockopt(m_fd, nLevel, nOptanme, Optval, OptLen);
	
	return 0 == nRet;
}

bool CSocket::GetSockOpt(int nLevel, int OptName, void* Optval,socklen_t* pOptLen)
{
	int nRet = getsockopt(m_fd, nLevel, OptName, Optval, pOptLen);
	return 0 == nRet;
}

void CSocket::SetBlockMode(int nMode/* = O_NONBLOCK*/)
{
	fcntl(m_fd, F_SETFL, nMode);
}
