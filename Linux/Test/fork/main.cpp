#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main()
{
	int pid = 0;
	printf("headool\n");
	pid = fork();
	if(pid == 0)
	{
		printf("child\n");
		_exit(0);
	}
	else if(pid > 0)
	{
		int status = 0;
		//wait(&status);
//
		printf("fork\n");
	}
	return 0;
}
