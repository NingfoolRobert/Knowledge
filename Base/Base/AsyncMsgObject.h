/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		AsyncMsgObject.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-12-05
 */

#pragma once 

#include "AsyncObject.h"
#include "BufferMgr.h"



template<class TYPE>
class CAsyncMsgObject: public CAsyncObject  
{
	typedef bool (TYPE:: *ASYNCMSGFUNC)(PMSGHEADER pMsg);
public:
	CAsyncMsgObject(void){}
	virtual ~CAsyncMsgObject(void) {}

public:
	bool Init(TYPE* pOwner, ASYNCMSGFUNC* pAsyncMsgFunc, int nDefaultThreadCount = 1, int nMixThreadCount  = 50)
	{
		if(pOwner == nullptr || pAsyncMsgFunc == nullptr)
		{
			return false;
		}
		
		if(!CAsyncObject::Init(nDefaultThreadCount, this))
		{
			return false;
		}	
	}
	
	virtual bool OnMsg(PMSGHEADER pMsg)
	{
		if(m_pOwner == nullptr || pMsg == nullptr) return false;
		return m_pOwner->*(m_pAsyncFunc)(pMsg);
	}

	bool AddAsyncMsg(PMSGHEADER pMsg)
	{
		if(pMsg == nullptr)
		{
			return false;
		}

		PostMsg(pMsg);

		return true;
	}
private:
	TYPE*				m_pOwner;
	ASYNCMSGFUNC		m_pAsyncFunc;
	int					m_nDefaultThreadCount;
	int					m_nMaxThreadCount;
	int					m_nCurrentThreadCount;
}
