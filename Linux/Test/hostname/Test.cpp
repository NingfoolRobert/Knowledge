#include <unistd.h>
#include <netdb.h>
#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>


int main()
{
	char szTmp[256] ={"www.baidu.com"};//{"smtp.163.com"};

	struct hostent *hptr = gethostbyname(szTmp);
	if(hptr == nullptr)
	{
		std::cout << "fail" << std::endl;
		
		switch (h_errno)
		{
			case HOST_NOT_FOUND:
				fputs ("The host was not found.\n", stderr);
				break;
			case NO_ADDRESS:
				fputs ("The name is valid but it has no address.\n", stderr);
				break;
			case NO_RECOVERY:
				fputs ("A non-recoverable name server error occurred.\n", stderr);			
				break; 
			
			case TRY_AGAIN:
				fputs ("The name server is temporarily unavailable.\n", stderr);		
				break;
																																							        
		}

		
		
		return 0;
	}
	//for(char* pptr = hptr->h_aliases;pptr != nullptr; pptr++)
	char str[1024]={0};
	char ** pptr = nullptr;
	switch(hptr->h_addrtype)
	{
		case AF_INET:
		pptr = hptr->h_addr_list;
		for (;*pptr != nullptr; pptr++ )
		{
			printf("\taddress: %s\n", inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
		}
		break;
		default:
			break;
	}

	return 0;
}
