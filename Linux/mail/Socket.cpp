#include "Socket.h"

CSocket::CSocket()
{
	m_fd = 0;
}
CSocket::~CSocket()
{
	Close();
}

int  CSocket::Create(int nDomain/* = AF_INET*/, int nSockType = /*SOCK_STREAM*/, int nProtocol/* = IPPROTO_IP*/)
{
	m_fd = socket(nDomain, nSockType, nProtocol);
	if(m_fd  < 0)
	{
		return 0;
	}

	return m_fd;
}

bool CSocket::Bind(int nHostPort/* = 0*/, const char* pszHostAddress = /*nullptr*/)
{
	struct sockaddr_in SvrAddr;
	memset(&SvrAddr, 0, sizeof(SvrAddr));
	
	SvrAddr.sin_family = AF_INET;
	//
	if(pszHostAddress == nullptr)
		SvrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else 
		SvrAddr.sin_addr.s_addr = inet_addr(pszAddress);
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
	memset(ClientAddr, 0, sizeof(ClientAddr));

	int sockLen = sizeof(ClientAddr);

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
	memset(&SvrAddr, 0, sizoef(SvrAddr));

	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = inet_addr(pszAddress);
	SvrAddr.sin_port = htons(nPort);

	if(connect(m_fd, (struct sockaddr*)&SvrAddr,sizeof(SvrAddr))  <= 0)
	{
		return false;
	}
	return true;
}

bool CSocket::Recv(char* pszBuf, int nBufLen)
{
	return true;
}

bool CSocket::Send(const char* pszBuf, int nBufLen)
{
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
	m_fd = nfd;
}

int  CSocket::Detach()
{
	return m_fd;
}
