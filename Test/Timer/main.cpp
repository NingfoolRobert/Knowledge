#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

#include <sys/time.h>
#include <sys/select.h> 


void Sleep(int ms)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = ms * 1000;
	select(0, NULL, NULL, NULL, &tv);
}

int main ()
{
	int nval = 1;
	printf("start:\n");
	while (true)
	{
		Sleep(10);
		printf("%d\n", nval++);
	}

	return 0;
}
