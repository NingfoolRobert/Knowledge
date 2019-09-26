#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>


void SignFun(int signo)
{
	time_t tNow = time(NULL);
	struct tm* pTime;
	localtime_r(&tNow, pTime);
	printf("%02d:%02d:%02d    ",pTime->tm_hour,pTime->tm_min,pTime->tm_sec);
	printf("sigFun is runnning\n");
}

int main()
{
	if(signal(SIGALRM, SignFun) == SIG_ERR)
	{
		perror("signal\n");
		return -1;
	}
	struct itimerval tv;
	tv.it_value.tv_usec = 500000;
	tv.it_value.tv_sec = 0;
	tv.it_interval.tv_usec = 500000;
	tv.it_interval.tv_sec = 0;
	if(setitimer(ITIMER_REAL, &tv, NULL) != 0)
	{
		perror("settimer\n");
		return -1;
	}
	
	while(true)
//	{
		pause();
//	}
	return 0;
}
