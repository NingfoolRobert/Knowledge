#include <iostream>
#include "ThreadPool.h"
#include <time.h>

#include <unistd.h>
#include <sys/time.h>


void OnTest(int nums)
{
	printf("OnTest:: nums: %d\n",nums);
}

int main()
{
	ThreadPool pool(5);

	pool.enqueue(&OnTest,10);

	usleep(100);
	auto task = std::bind(&OnTest,100);
	
	pool.enqueue(task);
	return 0;
}
