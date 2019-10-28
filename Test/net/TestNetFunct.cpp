#include <arpa/inet.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char szTmp[32] = { "192.168.15.30" };
	struct in_addr  addr;
	//点分十进制转网络字节序 IP
	
	char delim[] =".";
	char *ps = strdup(szTmp);
	unsigned int dwHostIP = 0;
	for(char* token = strsep(&ps, delim); token != nullptr; token =strsep(&ps, delim))
	{
		dwHostIP <<= 8;
		dwHostIP += atoi(token);
	}
	std::cout << dwHostIP << std::endl;
	
	char szOut[32] ={0};
	sprintf(szOut, "%d.%d.%d.%d", dwHostIP >> 24, (dwHostIP & 0xff0000) >> 16,(dwHostIP & 0xff00) >> 8, dwHostIP & 0xff);

	std::cout << szOut << std::endl;
	
	
	int nRet = inet_aton(szTmp, &addr);
	if(!nRet)
	{
		std::cout << nRet << std::endl;
	}
//	std::cout << addr << std::endl;

	unsigned int dwIP = ntohl (addr.s_addr);
	std::cout << std::hex << dwIP << std::endl;
	return 0;
}
