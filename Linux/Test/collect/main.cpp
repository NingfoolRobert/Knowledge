#include <unistd.h> 
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>

#include <string.h> 


void print(FILE* fp)
{
	char buf[256];
	if(!fp)
	{
		return ;
	}
	printf("\n>>>\n");
	while(memset(buf,0,sizeof(buf)),fgets(buf,sizeof(buf)-1,fp) != 0)
	{
		printf("%s",buf);;
	}
	printf("\n>>>\n");
}

int main()
{
	FILE *fp = nullptr;
	fp = popen("/usr/bin/python2.7 ./Xtest.py","r");
	if(!fp)
	{
		perror("popen");
		exit(EXIT_FAILURE);
	}
	print(fp);
	pclose(fp);
	sleep(1);

	return 0;
}
