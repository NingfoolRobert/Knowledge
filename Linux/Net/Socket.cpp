#include "Socket.h"
#include <unistd.h>

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
	m_addr.sin_port = htons(nHostPort);
	
	if(pszAddress == nullptr)
	{
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		m_addr.sin_addr.s_addr = htonl(pszAddress);
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
	m_addr.sin_addr.s_addr= htonl(pszHostAddress);

	socklen_t len = 0;
	int nRet = connect(m_fdSocket,(struct sockaddr*)&m_addr,&len);

	return true;
}

int  CSocket::Recv(void *pBuf, int nBufLen,int nFlags = 0, bool bRecvAll = false)
{
	
	int nRecv = read(m_fdSocket,pBuf,sizeof(HEADER),nFlags);
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
				nRecv = read(m_fdSocket,pBuf+ uRecved,nBufLen - uRecved,nFlags);

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
		int nSend = write(m_fdSocket, pBuf + nSended, nBufLen - nSended,nFlags);
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
	int Old_Option = fcntl(m_fdSocket,F_GETFL,)
	fcntl(m_fdSocket,F_SETFL,O_NONBLOCK);
	return true;
}
