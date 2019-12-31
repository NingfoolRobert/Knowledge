/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxTaskDefine.h
 * @brief:xxxTaskDefine 
 * @author:xxxTaskDefinenbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2019-12-25
 */


#pragma once 

#include "Thread.h"
#include "BufferMgr.h"

class CActiveObject;

typedef struct stTimerHeader
{
	unsigned int	dwTimerID;
	unsigned long	dwOwner;	
	unsigned long	lTimerOver;
	unsigned int	dwLength;

}TIMERHEADER, *PTIMERHEADER;

typedef struct stEventHeader 
{
	unsigned int	dwType;
	unsigned int	dwLength;
}EVENTHEADER, *PEVENTHEADER;

struct compTimer 
{
	bool operator()(const void* arg1, const void* arg2)
	{
		CBuffer* pBuf1 = (CBuffer*)arg1;
		CBuffer* pBuf2 = (CBuffer*)arg2;
		if(pBuf1 == nullptr || nullptr == pBuf2)
			return false;
		PTIMERHEADER pTimer1 = (PTIMERHEADER)pBuf1->GetBufPtr();
		PTIMERHEADER pTimer2 = (PTIMERHEADER)pBuf2->GetBufPtr();
		if(nullptr == pTimer1 || nullptr == pTimer2)
			return false;
		if(pTimer1->lTimerOver == pTimer2->lTimerOver)
		{
			if(pTimer1->dwTimerID == pTimer2->dwTimerID)
				return pTimer1->dwOwner < pTimer2->dwOwner;
			else 
				return pTimer1->dwTimerID < pTimer2->dwTimerID;
		}
		else 
		{
			return pTimer1->lTimerOver < pTimer2->lTimerOver;
		}
	
	}

};


class CTimer: public Runnable 
{
public:
	CTimer(CActiveObject* pObject, CBuffer* pBuf);	
	virtual ~CTimer(void);
public:
	virtual void Run();
private:
	CActiveObject*	m_pObject;
	CBuffer*		m_pBuf;
};

class CEvent: public Runnable 
{
public:
	CEvent(CActiveObject* pObj, CBuffer* pBuf);
	virtual ~CEvent();
public:
	virtual void Run();
private:
	CActiveObject*		m_pObject;
	CBuffer*			m_pBuf;
};

