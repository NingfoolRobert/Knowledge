/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		AcceptIO.h
 * @brief:		主要负责管理接受NetClient
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-25
 */

#include "NetIO.h"

class CNetService;
class CNetClient;

class CAcceptIO: public CNetIO 
{
public:
	CAcceptIO();
	virtual ~CAcceptIO();
public:
	virtual bool OnInitialUpdate(CNetService* pNetService);

	virtual void OnRecv();

public:

	bool Socket(int nPort, unsigned int dwIP = 0, int nDomain = AF_INET, int nSockType = SOCK_STREAM, int nProtocol = IPPROTO_IP);

	void AcceptThread();

protected:
	
	bool  AddNetClient(CNetClient* pNetClient);
	
private:
	CNetService*		m_pNetService;
};

