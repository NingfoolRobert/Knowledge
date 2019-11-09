

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
	char szTmp[20] = { "\r\n" };
	printf("%d\n", strlen(szTmp));
	

	int nPageSize = sysconf(_SC_PAGESIZE);
	printf("PageSize : %d\n", nPageSize);
	int file = open("./Test.txt",O_RDWR);
	printf("%d\n", file);
	if(file <= 0)
	{
		printf("Open file fail.\n");
		return 0;
	}
	char* pHead = (char*)mmap(0, sizeof(char) * 1024 * 1024 * 1024,PROT_READ | PROT_WRITE, MAP_PRIVATE, file, 0);
	if(nullptr == pHead)
	{
		printf("Read File fail.");
		return 0;
	}
	int nlen = strlen(pHead);
	printf("Data Len: %d\n", nlen);
	char* pEnd = strchr(pHead, '\n');
	if(nullptr != pEnd)
	{
		*pEnd = 0;
//		printf("End: %s\n", ++pEnd);
	}
	while(1)
		;
//	printf("File Txt: %s\n", pHead);
//	msync(pHead, sizeof(char)* 1024, MS_ASYNC);
	munmap(pHead, sizeof(char*) * 1024);
	close(file);
	
	return 0;
}
