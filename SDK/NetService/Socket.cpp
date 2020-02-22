#include "Socket.h"
#include <memory>
#include <string.h>

CSocket::CSocket()
{
	m_fd = -1;
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

	
bool CSocket::Bind(int nHostPort/* = 0*/, unsigned int dwHostIP/* = 0*/)
{
	struct sockaddr_in SvrAddr;
	memset(&SvrAddr, 0, sizeof(SvrAddr));
	
	SvrAddr.sin_family = AF_INET;
	//
	if(dwHostIP == 0)
		SvrAddr.sin_addr.s_addr = INADDR_ANY;
	else 
		SvrAddr.sin_addr.s_addr = htonl(dwHostIP);

	SvrAddr.sin_port = htons(nHostPort);

	if(bind(m_fd, (struct sockaddr*)&SvrAddr,sizeof(SvrAddr)) < 0)
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

	return  accept(m_fd, (struct sockaddr*)&ClientAddr,&sockLen);
}

bool CSocket::Connect(unsigned int dwHostIP, int nPort)
{
	struct sockaddr_in SvrAddr;
	memset(&SvrAddr, 0, sizeof(SvrAddr));

	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = htonl(dwHostIP);		//主机字节序转网络字节序
	SvrAddr.sin_port = htons(nPort);
	if(connect(m_fd, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) < 0)
	{
		return false;
	}
	return true;
}

int CSocket::Recv(char* pszBuf, int nBufLen)
{
	return read(m_fd, pszBuf, nBufLen);
}

int CSocket::Send(const char* pszBuf, int nBufLen)
{
	return write(m_fd, pszBuf, nBufLen);
}

void CSocket::Close()
{
	if(m_fd != -1)
	{
		shutdown(m_fd, SHUT_RDWR);
		close(m_fd);
	}
	m_fd = -1;
}

bool CSocket::Attach(int nfd)
{
	if(nfd <= 0)
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
	return setsockopt(m_fd, nLevel, nOptanme, Optval, OptLen) == 0;
}

bool CSocket::GetSockOpt(int nLevel, int OptName, void* Optval,socklen_t* pOptLen)
{
	return getsockopt(m_fd, nLevel, OptName, Optval, pOptLen) == 0;
}

void CSocket::SetBlockMode(int nMode/* = O_NONBLOCK*/)
{
	fcntl(m_fd, F_SETFL, nMode & fcntl(m_fd, F_GETFL));
}
