#include "Socket.h"


CSocket::CSocket()
{

}
CSocket:: ~CSocket()
{

}

bool CSocket::Create(unsigned int nPort = 0, const char* pszAddress = nullptr, int nSocketType=SOCK_STREAM, int nProtocl= AF_INETi)
{
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

int  CSocket::Send(const void* pBuf ,int nBufLen, int nFlags = 0)
{
	return 0;
}

bool CSocket::Close()
{
	return 0;
}
ic:

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
	return true;
}
