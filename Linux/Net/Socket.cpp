#include "Socket.h"


CSocket::CSocket()
{

}
CSocket:: ~CSocket()
{

}

bool CSocket::Create(unsigned int nPort /*= 0*/, const char* pszAddress /*= nullptr*/, int nSocketType/* = SOCK_STREAM*/, int nProtocl /*= AF_INET*/)
{
	m_addr.sin_family	= nPortocol;
	m_addr.sin_port		= htons(nPort);
	if(pszAddress == nullptr)
	{
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		m_addr.sin_addr.s_addr = htonl(pszAddress);
	}
	m_fdSocket = socket(nPortocol,nSocketType,0);
	if(m_fdSocket <= 0)
	{
		//TODO
		return false;
	}
	return true;
}

bool CSocket::Bind(unsigned int nHostPort = 0, const char* pszHostAddress = nullptr)
{
	return true;
}

bool CSocket::Connect(const char* pszHostAddress, unsigned int nPort)
{
	return true;
}

int  CSocket::Recv(void *pBuf, int nBufLen,int nFlags = 0, bool bRecvAll = false)
{
	return 0;
}

int  CSocket::Send(const void* pBuf ,int nBufLen, int nFlags/* = 0*/)
{
	int nSended = 0;
	while(nSended < nBufLen)
	{		
		int nSend = send(m_fdSocket, pBuf + nSended, nBufLen - nSended,nFlags);
		if(nSend < 0)
		{
			//TODO  log
			return nSended;
		}
		nSended += nSend;
	}
	return nBufLen;
}

bool CSocket::Close()
{
	close(m_fdSocket);
	return 0;
}

bool CSocket::Attach(int fd_socket)
{
	return true;
}

int  CSocket::Detach()
{
	return 0;
}

bool CSocket::CSocket::GetPeerName()
{

}

bool CSocket::SetNonBlocking()
{
	fcntl(m_fdSocket,F_SETFL,O_NONBLOCK);
	return true;
}
