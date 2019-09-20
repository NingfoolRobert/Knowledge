#pragma once

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>


class sem_locker
{
public:
	sem_locker()
	{
		if(sem_init(&m_sem,0,0) != 0)
			printf("sem init error");
	}
	~sem_locker()
	{

		sem_destory(&m_sem);
	}
	bool wait()
	{
		return sem_wait(&m_sem) == 0;
	}
	bool add()
	{

		return sem_post(&m_sem) == 0;
	}

private:
	sem_t m_sem;
};


class  CObjectLock
{
public:
	CObjectLock() 
	{
		int rc = pthread_mutex_init(&m_mutex, nullptr) ;
		assert(rc == 0);
	}
	~CObjectLock()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	void Lock()
	{
		pthread_mutex_lock(&m_mutex);
	}
	void UnLock()
	{
		pthread_mutex_unlock(&m_mutex);
	}

private:
	pthread_mutex_t  m_mutex;
};


class Cond_Lock
{
public:
	Cond_Lock()
	{
		int nRet =	pthread_mutex_init(&m_mutex, nullptr);
		assert(nRet == 0);
		nRet = pthread_cond_init(&m_cond,nullptr);
		assert(nRet == 0);
	}
	~Cond_Lock()
	{

		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
	}

	bool Wait()
	{

		int ans = 0;
		pthread_mutex_lock(&m_mutex);
		ans = pthread_cond_wait(&m_cond,&m_mutex);
		pthread_mutex_unlock(&m_mutex);
		return ans == 0;
	}
	bool signal()
	{

		return pthread_cond_signal(&m_cond) == 0;
	}
	bool broadcast()
	{
		return pthread_cond_broadcast(&m_cond) == 0;
	}

private:
	pthread_mutex_t m_mutex;
	pthread_cond_t	m_cond;
};
