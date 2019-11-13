/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		Thread.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-11-13
 */
#pragma once 

#include <stdio.h>
#include <pthread.h> 

typedef void*(*PTHREADFUNC)(void*);

enum 
{
	RUNMODE_DETACH, 
	RUNMODE_JOIN,
	RUNMODE_COUNT,
};


class CThread
{
public:
	CThread();
	~CThread();
public:

	bool Init(PTHREADFUNC fnc, void* pArgs = nullptr, pthread_attr_t* pAttr = nullptr);

	bool Run(int nMod = RUNMODE_DETACH);

	bool Join();

	bool Detach();

	pthread_t GetThreadID() { return m_tid; }
private:

	pthread_t			m_tid;
	pthread_attr_t*		m_pAttr;
	void*				m_pArgs;	
	PTHREADFUNC			m_fnc;
};
