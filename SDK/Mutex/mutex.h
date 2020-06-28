/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		mutex.h
 * @brief:		mutex
 * @author:		mutexnbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2020-06-28
 */

#include <pthread.h> 



class CSpinLock
{
public:
	CSpinLock(){ pthread_spinlock_init(&m_spin, PTHREAD_PROCESS_PRIVATE); }
	~CSpinLock(){
		pthread_spinlock_destroy(&m_spin);
	}
	
	void Lock(){
		pthread_spin_lock(&m_spin);
	}

	void UnLock(){
		pthread_spin_unlock(&m_spin);
	}
private:
	pthread_spinlock_t		m_spin;
};

class CAutoSpinLock 
{
public:
	CAutoSpinLock(CSpinLock& _spinlock){
		m_pSpinlock = &_spinlock;
		m_pSpinlock->Lock();
	}	
	~CAutoSpinLock(){
		m_pSpinlock->UnLock();
		m_pSpinlock = nullptr;
	}
private:
	CSpinLock*		m_pSpinlock;
};
