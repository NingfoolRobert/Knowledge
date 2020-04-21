/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxActiveObject.h
 * @brief:xxxActiveObject 
 * @author:xxxActiveObjectnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2019-12-25
 */

#pragma once 

#include <set>
#include <map>
#include <mutex>
#include <condition_variable>
#include "AsyncObject.h"
#include "PtrCycle.h"

class CActiveObject: 
	public CAsyncObject
{
public:
	CActiveObject(void);
	virtual ~CActiveObject(void);
public:
	bool Init(int cnThread = 1, CActiveObject* pOwner = nullptr);

	CActiveObject* GetOwner(){return m_pOwner;}
	
	void Terminate();

	int GetThreadCnt(){return m_cnThread;};
	
	int GetWaitEventCnt(){return m_listEvent.GetCount();}

public:
	virtual bool PostEvent(PEVENTHEADER pEvent);

	virtual bool PostEvent(CBuffer* pBuffer);
public:
	void ActiveThreadFunc();
private:
	bool										m_bStop;
	std::mutex									m_clsLock;
	std::condition_variable						m_condEvent;
	CPtrCycle<CBuffer>							m_listEvent;
private:
	int											m_cnThread;
	CActiveObject*								m_pOwner;
};
