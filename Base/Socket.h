/// @file Socket.h
/// @brief 
/// @author ningbf,Beifei.Ning@cicc.com.cn
/// @version 
/// @date 2019-10-24


#pragma once 

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>



class CSocket
{
public:
	CSocket();
	virtual ~CSocket();
public:
	int Create(int nDomain = AF_INET, int nSockType = SOCK_STREAM, int nProtocol = IPPROTO_IP);
	
	bool Bind(int nHostPort = 0, const char* pszHostAddress = nullptr);

	bool Bind(int nHostPort = 0, unsigned int dwHostIP = 0);
	
	bool Listen(int nListenCount = 10);

	int  Accept();

	bool Connect(const char* pszAddress, int nPort);

	bool Connect(unsigned int dwHostIP, int nPort);

	int Recv(char* pszBuf, int nBufLen);

	bool Send(const char* pszBuf, int nBufLen);
	
	void Close();
public:
	bool Attach(int nfd);

	int  Detach();

	bool GetSockName(struct sockaddr* LocalSockaddr, socklen_t* nAddrLen);

	bool GetPeerName(struct sockaddr* PeerSockAddr, socklen_t* nAddrLen);

	bool SetSockOpt(int nLevel, int nOptanme, const void* Optval, socklen_t pOptLen);
	
	bool GetSockOpt(int nLevel, int OptName, void* Optval,socklen_t* pOptLen);
	
	void SetBlockMode(int nMode = O_NONBLOCK);

protected:
	int		m_fd;		//Socket handle 
};
