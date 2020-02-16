/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		Connection.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-02-10
 */


#pragma once 

#include "Protocol.h"
#include "Socket.h"
#include "ObjectLock.h"
#include "Buffer.h"


#include <queue>

typedef struct stAddrInfo
{
	char			szIP[16];
	unsigned int	dwHostIP;
	unsigned int	dwNetIP;
	int				nPort;
}ADDRINFO, *PADDRINFO;


class CConnection:public CSocket
{
public:
	CConnection();
	virtual ~CConnection();
public:
	bool InitialUpdate(unsigned int dwHostIP, int nPort);

	bool CheckConnectState(bool bReconnect = true);
	
	bool SendMsg(PHEADER pHeader);
	
	PHEADER PeerMsg();

	bool SendMsg(PHEADER pHeader, bool bAck, int nOrgine, unsigned int dwAckType, CBuffer* pBuffer);
	
	bool SendMsg(unsigned int dwHostIP, int nPort, PHEADER pHeader);
protected:
	PHEADER				m_pHeader;
	CBuffer*			m_pBuf;
private:
	PADDRINFO			m_pAddrInfo;	
};


//
class CConnectionPool 
{
public:
	CConnectionPool();
	virtual ~CConnectionPool();
public:
	bool InitialUpdate(const char* pszIP, int nPort, int nConnCount = 3);

	bool SetAddrInfo(const char* pszIP, int nPort);
	
	bool SetAddrInfo(PADDRINFO* pAddrInfo);
	
	bool SetAddrInfo(unsigned int dwHostIP, int nPort);

	bool Terminate();
public:
	CConnection* GetConnection();
	void		 ReclaimConn(CConnection* pConn);
private:
	
	CObjectLock					m_clsLock;
	std::queue<CConnection*>	m_listConnection;
private:
	PADDRINFO					m_pAddrInfo;
	int							m_nConnMax;
	bool						m_bClose;
};
