


#include "ObjectLock.h"
#include "AutoLock.h"


#include <stdio.h> 
#include <sys/time.h> 
#include <mutex>

std::mutex  m_mutex;

class CObjectLock m_clsLock;
void TestLocker()
{
	CAutoLock locker(&m_clsLock);
	static long n= 0;
	n++;
}
void TestLocker1()
{
	static long n= 0;
	m_clsLock.Lock();
	n++;
	m_clsLock.UnLock();
}

void TestMutex()
{
	std::unique_lock<std::mutex> locker(m_mutex);

	static int p =0;
	p++;
}


int main()
{
	struct timeval tStart, tEnd;
	gettimeofday(&tStart, NULL);
	for(long i = 0; i < 1000000; ++i)
	{
		TestLocker();
	}
	gettimeofday(&tEnd, NULL);

	int64_t  diff = (tEnd.tv_sec - tStart.tv_sec) * 1000000LL  + tEnd.tv_usec - tStart.tv_usec;

	printf("locker diff:%lld\n", diff);

	gettimeofday(&tStart, NULL);
	for(long i = 0; i < 1000000; ++i)
	{
		TestLocker();
	}
	gettimeofday(&tEnd, NULL);

	diff = (tEnd.tv_sec - tStart.tv_sec) * 1000000LL  + tEnd.tv_usec - tStart.tv_usec;

	printf("mutex, diff:%lld\n", diff);

	gettimeofday(&tStart, NULL);
	for(long i = 0; i < 1000000; ++i)
	{
		TestLocker();
	}
	gettimeofday(&tEnd, NULL);

	diff = (tEnd.tv_sec - tStart.tv_sec) * 1000000LL  + tEnd.tv_usec - tStart.tv_usec;

	printf("clock, diff:%lld\n", diff);


	return 0;
}



