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
#include "../Buffer.h"
#include "Thread.h"
#include "../BufferMgr.h"

class CActiveObject;

typedef struct stTimerHeader
{
	unsigned int	dwTimerID;
	
	long			lTimerOver;
	unsigned int	dwLength;

}TIMERHEADER, *PTIMERHEADER;

typedef struct stEventHeader 
{
	unsigned int	dwType;

	unsigned int	dwLength;
}EVENTHEADER, *PEVENTHEADER;


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

