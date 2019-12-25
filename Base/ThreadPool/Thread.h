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
#include <stdlib.h>
#include <pthread.h> 

enum 
{
	RUNMODE_DETACH, 
	RUNMODE_JOIN,
	RUNMODE_COUNT,
};

#define CLASS_UNCOPYABLE(classname) \
        private:\
        classname##(const classname##&);\
        classname##& operator=(const classname##&);

class Runnable
{
public:
    virtual void Run()=0;
    virtual ~Runnable(){}
};

class CThread: public Runnable 
{
public:
	CThread(Runnable* pTarget = nullptr);
	~CThread();
public:
	
	bool Start(int nMod = RUNMODE_DETACH);

	bool Join();

	bool Detach();

	pthread_t GetThreadID() { return m_tid; }

	virtual void Run() {}

	unsigned int  ThreadProc();
private:
	
	Runnable*			m_pTarget;
	pthread_t			m_tid;
};
