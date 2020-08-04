/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		AcceptIO.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-16
 */


#pragma once 

#include "NetIO.h"


class CAcceptIO:
	public CNetIO 
{
public:	
	CAcceptIO();
	virtual ~CAcceptIO();
public:
	virtual bool OnRecv();
	
	virtual bool OnClose();
public:
	virtual bool IsReadable();

	virtual void ForbidRead();
	
	virtual void PermitRead();
public:
	bool OnInit(int nPort, unsigned int dwHostIP = 0);
	
	void Terminate();
private:
	bool	m_bStop;
};
