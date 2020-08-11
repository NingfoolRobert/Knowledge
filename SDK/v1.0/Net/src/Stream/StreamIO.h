/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		StreamIO.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-09
 */

#pragma once 
#include "NetIO.h"

#include <vector> 
#include <mutex> 
#include <list>


typedef std::vector<char> Buffer;

class CStreamIO :
	public CNetIO
{
public:
	explicit CStreamIO();
	virtual ~CStreamIO();
public:
	virtual bool OnSend();
	
	virtual bool OnRecv();

	virtual bool OnClose();
public:
	virtual void ProcessMsg();
private:
	std::mutex			m_clsRecvLock;
	char*				m_pRecvBuf;
	unsigned int		m_nCapbility;
	unsigned int		m_dwRecvLen;

	std::mutex			m_clsSendLock;
	

};
