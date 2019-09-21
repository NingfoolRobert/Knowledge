#pragma once

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
	bool Create(int domain = PF_INET, int nSocketType=SOCK_STREAM, int nProtocol = 0);

	bool Bind(unsigned int nHostPort = 0, const char* pszHostAddress = nullptr);

	bool Connect(const char* pszHostAddress, unsigned int nPort);
	
	int  Recv(void *pBuf, int nBufLen,bool bRecvAll = false);
	
	int  OnRecv(void* pBuf, int nBufLen);

	int  Send(const void* pBuf ,int nBufLen);

	bool Close();
public:
	
	bool Attach(int fd_socket);

	int  Detach();
public:
	bool GetPeerName(char* pszAddress,int *pPeerPort);

	bool GetSockName(char* pszAddress,int *pSockPort);

	void SetNonBlocking();

	void SetTimeOut(int nSecond);		//设置发送和接受的超时事件(秒)

	void SetNoDelay(bool bNoDelay);	//设置禁用Nagle算法

	void SetLinger(int nSecond);		// 设置容许数据逗留时间

	void SetKeepAlive(bool bKeepAlive);

	int	 GetSockFD(){ return m_fdSocket; }
protected:
	//void SetSockOpt();
	//
	bool SetSockOpt(int nOptionName, const void* pOptionValue, int nOptionLen, int nLevel = SOL_SOCKET);
public:
	int					m_fdSocket;
//	int					m_nPort;
//	struct sockaddr_in	m_addr;

//	bool				m_bInit;


private:
	
	static int			m_nDefaultRecvBufSize;			//默认接收缓冲区大小(0表示未修改) (16384)

	static int			m_nDefaultSendBufSize;			//默认发送缓冲区大小(0表示未修改)(87380)

	static int			m_nDefaultTimeout ;				//默认超时时间(0表示未修改)

};
