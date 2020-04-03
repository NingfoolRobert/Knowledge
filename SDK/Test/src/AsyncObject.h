/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxAsyncObject.h
 * @brief:xxxAsyncObject 
 * @author:xxxAsyncObjectnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-04-03
 */


#pragma once 

#include "Available.h"
#include "Buffer.h"
#include "EventDef.h"



class CAsyncObject :public CAvailable 
{
public:
	CAsyncObject();
	virtual ~CAsyncObject();
public:
	
	virtual bool OnEvent(PEVENTHEADER pEvent);
	virtual bool PostEvent(PEVENTHEADER pEvent);
	virtual bool PostEvent(CBuffer* pBuffer);

	virtual bool OnTimer(PTIMEHEADER  pTimer);
	virtual bool SetTimer(PTIMEHEADER pTimer, int nSecond);
	virtual bool SetTimerMilli(PTIMEHEADER pTimer, int nMilliSec);
	virtual bool KillTimer(unsigned int dwTimerID, int64_t pTimerOwner = 0);
public:
	CAsyncObject* GetOwner(){return m_pOwner;}
	void		 SetOwner(CAsyncObject* pOwner){m_pOwner = pOwner;}
private:
	CAsyncObject*	m_pOwner;
};

