#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>


class CSocket
{
public:
	CSocket();
	virtual ~CSocket();
public:
	int Create(int nDomain = AF_INET, int nSockType = SOCK_STREAM, int nProtocol = IPPROTO_IP);
	
	bool Bind(int nHostPort = 0, const char* pszHostAddress = nullptr);

	bool Listen(int nListenCount = 10);

	int  Accept();

	bool Connect(const char* pszAddress, int nPort);

	bool Recv(char* pszBuf, int nBufLen);

	bool Send(const char* pszBuf, int nBufLen);
	
	void Close();

	bool Attach(int nfd);

	int  Detach();
private:
	int		m_fd;		//Socket handle 
}
