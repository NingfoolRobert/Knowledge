/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		Connection.h
 * @brief:		主动网络链接实现
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-28
 */


#pragma once 

#include "NetIO.h"
#include "Protocol.h"
#include "Buffer.h"


class  CConnection: public CNetIO 
{
public:
	CConnection();
	virtual ~CConnection();
public:
	
	bool SendMsg(unsigned int dwIP, int nPort, PHEADER pMsg, bool bWaitRecv = false);

	PHEADER PeerMsg();
private:
	PHEADER			m_pHeader;
	
	unsigned int	m_dwIP;
	int				m_nPort;
	CBuffer*		m_pbufRecv;
};
