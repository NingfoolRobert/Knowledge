#include <string.h> 
#include <cstdlib> 
#include <cstdio> 
#include <iostream> 




int main()
{
	const char* pszSplite = ";:";
	char szTmp[64] = { "192.168.18.183:30003; 192.168.189.23:22000;" };

	char* pSave = NULL;
	char* ptr  =  strtok_r(szTmp, pszSplite, &pSave);
	while(ptr)
	{
		std::cout << ptr<< std::endl;
		ptr = strtok_r(NULL, pszSplite, &pSave);
	}
	return 0;
}
