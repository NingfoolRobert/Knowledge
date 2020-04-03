/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		Lock.h
 * @brief:		Lock 
 * @author:		nbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2020-03-03
 */


#pragma once 

#include <pthread.h> 
#include <sys/types.h> 
#include <errno.h> 


class CObjectLock 
{
public:
	explicit CObjectLock(bool bRecursive = false)
	{
		pthread_mutexattr_t Attr;
		pthread_mutexattr_init(&Attr);
		pthread_mutexattr_settype(&Attr, bRecursive ? PTHREAD_MUTEX_RECURSIVE : PTHREAD_MUTEX_NORMAL);
		pthread_mutex_init(&m_cls, &Attr);
		pthread_mutexattr_destroy(&Attr);
	}

	~CObjectLock()
	{
		while(pthread_mutex_destroy(&m_cls) == EBUSY) 
		{
			Lock();
			UnLock();
		}
	}

	void Lock()
	{
		pthread_mutex_lock(&m_cls);
	}

	void UnLock()
	{
		pthread_mutex_unlock(&m_cls);
	}

private:
	pthread_mutex_t			m_cls;
};

//
class CRWObjectLock 
{
public:
	CRWObjectLock()
	{
		pthread_rwlock_init(&m_cls, nullptr);
	}

	~CRWObjectLock()
	{
		while(pthread_rwlock_destroy(&m_cls) == EBUSY) 
		{
			WRLock();
			UnLock();
		}
	}

	class CWRAutoLock 
	{
	public:
		CWRAutoLock(CRWObjectLock& lock):m_clsRW(&lock)
		{
			m_clsRW->WRLock();
		}
		~CWRAutoLock()
		{
			m_clsRW->UnLock();
		}

	private:
			 CRWObjectLock*		m_clsRW;
	};
	
	class CRDAutoLock 
	{
	public: 
		CRDAutoLock(CRWObjectLock& lock):m_clsRD(&lock)
		{
			m_clsRD->RDLock();
		}

		~CRDAutoLock()
		{
			m_clsRD->UnLock();
		}
	private:
		CRWObjectLock*	m_clsRD;
	};
public:
	void WRLock() 
	{
		pthread_rwlock_wrlock(&m_cls);
	}

	void RDLock()
	{
		pthread_rwlock_rdlock(&m_cls);
	}

	void UnLock()
	{
		pthread_rwlock_unlock(&m_cls);
	}
private:
	pthread_rwlock_t		m_cls;
};
