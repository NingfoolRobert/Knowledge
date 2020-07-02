#include "mutex.h"

#include <thread> 

#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 



unsigned int dwNum = 0;

CSpinLock lock;

void Test()
{
	while(true)
	{
		lock.Lock();
		dwNum++;
		lock.UnLock();
		printf("1:%u\n", dwNum);
//		usleep(1 * 1000 * 100);
	}
}

void Test1()
{
	while(true)
	{
		lock.Lock();
		dwNum++;
		lock.UnLock();
		printf("2: %u\n", dwNum);
//		usleep(1 * 1000 * 100);
	}
	
}

int main()
{

	std::thread tr1(Test);
	std::thread tr2(Test1);
	tr1.join();
	tr2.join();
	
	return 0;
}
