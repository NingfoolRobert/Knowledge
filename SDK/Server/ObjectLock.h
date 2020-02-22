/*!==========================================
 * \File:	ObjectLock.h
 * \Date: 	2018-9-14  22:59
 *
 * \Author:  foolRobert
 * \Contact: 371494724@qq.com
 *
 * \Brief:   
 *
 * \TODO: 	Relize the Lock with the CRITICAL_SECTION  
 *
 * \Note:
*===========================================*/
#pragma once

#ifdef WIN32
#include <Windows.h> 
#include <assert.h>
#define  LOCK_TYPE  CRITICAL_SECTION
#endif

#ifdef __GNUC__
#include <sys/types.h>
#include <pthread.h>
#include <assert.h> 
#define LOCK_TYPE  pthread_mutex_t
#endif

class CObjectLock
{
public:
    CObjectLock(void)
	{
#ifdef WIN32
		::InitializeCriticalSection(&m_lock);
#endif

#ifdef __GNUC__
		::pthread_mutex_init(&m_lock, NULL);
#endif

	}
    virtual ~CObjectLock(void)
	{
#ifdef WIN32
		::DeleteCriticalSection(&m_lock);
#endif
#ifdef __GNUC__
		::pthread_mutex_destroy(&m_lock);
#endif
	}
public:
    void Lock()
	{
#ifdef WIN32
		::EnterCriticalSection(&m_lock);
#endif

#ifdef __GNUC__
		pthread_mutex_lock(&m_lock);
#endif
	}
    void UnLock()
	{
#ifdef WIN32
		::LeaveCriticalSection(&m_lock);
#endif
#ifdef __GNUC__
		pthread_mutex_unlock(&m_lock);
#endif
	}
    
private:
    LOCK_TYPE  m_lock;
};
