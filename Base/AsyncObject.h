/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		AsyncObject.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-12-02
 */

#pragma once 

#include "MsgDefine.h"
#include "Buffer.h"

#include <mutex>
#include <queue>
#include <condition_variable> 

class CAsyncObject
{
public:
	CAsyncObject(void);
	virtual ~CAsyncObject(void);
public:
//	virtual bool OnInitialUpdate();

	bool Init(int nThread = 1, CAsyncObject* pOwner = nullptr);

	virtual	bool PostMsg(PMSGHEADER pMsg);

	virtual bool PostMsg(CBuffer* pBuffer);

	virtual bool OnMsg(PMSGHEADER pMsg);

	virtual void Terminate();
public:
	bool AsyncMsgThread();

	bool SetOwner(CAsyncObject* pOwner);
private:
	bool							m_bStop;
	std::mutex						m_clsLock;
	std::condition_variable			m_condLock;
	std::queue<CBuffer*>			m_listMessage;
	CAsyncObject*					m_pAsyncObj;
};
extern class CAsyncObject* g_ciccAsyncObject; 
