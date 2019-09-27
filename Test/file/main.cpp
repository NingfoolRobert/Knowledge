#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <iostream>
#include <unistd.h>
#include <time.h>

using namespace std;

int main()
{
	FILE* fp = fopen("/home/nbf/Test.log","a+");
	if(nullptr == fp)
	{
		cout << "open fail.."<<endl;
		return -1;
	}
	
	char szTmp[100]= {"dninhao\n"};
	int ncount = 100;
	while (ncount--)	
	{
		fwrite(szTmp,1,strlen(szTmp),fp);
		usleep(1000000);
	}
	fclose(fp);

	return 0;
}
