

#include <iostream>
#include <stdio.h>
#include <unistd.h> 
#include <string.h>


using namespace std;

int main ()

{
	FILE* pFile = fopen("./Test.txt","w");

	if(nullptr == pFile)
	{
		cout << "open file fail." << endl;
		return 0;
	}

	char buf[128] = { "djafj;ldjkaklfjalsdjflajsldjf;lasjkdfajskdfa\n" };
	while (1)
	{
		int nSize =	fwrite(buf, 1,strlen(buf), pFile);
		cout << nSize << endl;
		usleep(1000000);
		fflush(pFile);		
	}
	fclose(pFile);
	getchar();
	return 0;

}
