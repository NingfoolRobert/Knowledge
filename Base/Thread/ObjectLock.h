/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * except with written permission of xxx.
 * 
 * @file:    ObjectLock.h
 * @brief:   ObjectLock 
 * @author:  ObjectLocknbf,nbf0537@163.com
 * @version: 1.0
 * @date:    2019-12-01
 */


#include <pthread.h>

class CObjectLock
{
public:
	
	CObjectLock(void)
	{
		pthread_mutex_init(&m_clsLock, NULL);
	}

	~CObjectLock(void)
	{
		pthread_mutex_destroy(&m_clsLock);
	}
public:
	void Lock()
	{
		pthread_mutex_lock(&m_clsLock);
	}

	void UnLock()
	{
		pthread_mutex_unlock(&m_clsLock);
	}

private:
	pthread_mutex_t			m_clsLock = PTHREAD_MUTEX_INITIALIZER;		
};


