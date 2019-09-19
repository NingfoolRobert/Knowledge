#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>


class CSocket
{
public:
	CSocket();
	virtual ~CSocket();
public:
	bool Create(unsigned int nPort = 0, const char* pszAddress = nullptr, int nSocketType=SOCK_STREAM, int nProtocl= AF_INET,);
	bool Bind(unsigned int nHostPort = 0, const char* pszHostAddress = nullptr);

	bool Connect(const char* pszHostAddress, unsigned int nPort);
	
	int  Recv(void *pBuf, int nBufLen,int nFlags = 0, bool bRecvAll = false);

	int  Send(const void* pBuf ,int nBufLen, int nFlags = 0);

	bool Close();
public:
	
	bool Attach(int fd_socket);

	int  Detach();
	
	bool GetPeerName(const char* pszAddress,int *pPeerPort);

	bool GetSockName(const char* pszAddress,int *pSockPort);

	bool SetNonBlocking();

private:
	int					m_fdSocket;
	struct sockaddr_in	m_addr;
};
