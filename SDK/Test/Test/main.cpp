

#include "Test.h"
#include <unistd.h> 
#include <sys/time.h> 
#include <stdio.h> 
#include <stdlib.h> 




int main()
{

	CTest tst;
	tst.OnInitialUpdate();

	struct timeval tnow;
	gettimeofday(&tnow, NULL);
	
	for(int i = 0; i < 1000; ++i)
	{
		tst.TestEvent();
	}
	struct timeval tend;
	gettimeofday(&tend, NULL);
	
	printf("%ld\n", (tend.tv_sec-tnow.tv_sec) * 1000000LL + tend.tv_usec - tnow.tv_usec);

	gettimeofday(&tnow, NULL);
	
	for(int i = 0; i < 100000; ++i)
	{
		tst.TestAlloc();
	}
	gettimeofday(&tend, NULL);
	
	printf("%ld\n", (tend.tv_sec-tnow.tv_sec) * 1000000LL + tend.tv_usec - tnow.tv_usec);


	usleep(10 * 1000 * 1000);


	tst.PrintInfo();


	return 0;
}
