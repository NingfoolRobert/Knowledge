#include <sys/time.h>
#include <time.h>
#include <sys/select.h>
#include <stdio.h>



void SetTimer(int seconds, int mseconds)
{
	struct timeval tmp;
	tmp.tv_sec = seconds;
	tmp.tv_usec = mseconds;

	select(0, NULL, NULL, NULL, &tmp);
	printf("timer\n");
}

int main ()
{
	for(int i = 0; i < 100; ++i)
	{
		SetTimer(1,0);
	}
	return 0;
}
