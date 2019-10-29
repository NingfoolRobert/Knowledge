#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{

	int sockfd = 0;
	struct sockaddr_in SvrAdd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd <= 0)
	{
		std::cout << "socket error" << std::endl;
		return -1;
	}
	SvrAdd.sin_family = AF_INET;
	SvrAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
	SvrAdd.sin_port = htons(30030);

	if(connect(sockfd, (struct sockaddr*)&SvrAdd, sizeof(SvrAdd)) < 0)
	{
		std::cout << "connect error" << std::endl;
		return -1;
	}
	char szTmp[32] ={"ninhao"};
	
	int nWrite = send(sockfd, szTmp, 7,0);
	if(nWrite < 7)
	{
		std::cout << "send fail" << std::endl;
		return -1;
	}
	return 0;
}
