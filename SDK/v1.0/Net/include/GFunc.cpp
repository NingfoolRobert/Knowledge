#include "GFunc.h"
#include <string.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <unistd.h> 






unsigned int Str2HostIP(const char* pszIP)
{
	unsigned int dwHostIP  = inet_network(pszIP);
	return dwHostIP;
}


unsigned int Net2HostIP(unsigned int dwNetIP)
{
	return ntohl(dwNetIP);
}

char* HostIP2Str(unsigned int dwHostIP, char* pszIP)
{
	if(nullptr == pszIP)
		return nullptr;
	struct in_addr addr;
	unsigned int dwNetIP = htonl(dwHostIP);
	memcpy(&addr, &dwNetIP, sizeof(dwNetIP));
	strcpy(pszIP, inet_ntoa(addr));
	return pszIP;
}
