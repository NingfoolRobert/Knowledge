#include <sys/un.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h> 
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>


#define MAXLINE 255

int main(int argc, char **argv)
{
	
	int                    sockfd;
	char                recvline[MAXLINE + 1];
	struct sockaddr_in    servaddr;
	
	if (argc != 2)
		//err_quit("usage: a.out <IPaddress>");
		printf("usage: a.out <IPaddress>");
	
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		//err_sys("socket error");
		printf("socket error");
	
	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	
	servaddr.sin_port   = htons(30030);   
	
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	
//		err_quit("inet_pton error for %s", argv[1]);
	printf("inet_pton error for %s", argv[1]);
	
	if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	//	err_sys("connect error");
	printf("connect error\n");
	char input[100];
	while (fgets(input, 100, stdin) != EOF)
	{
		write(sockfd, input, strlen(input));
		int n = 0;
		int count = 0;
		while (1)
		{
			n = read(sockfd, recvline + count, MAXLINE);
			if (n == MAXLINE)
			{
				count += n;
				continue;
			}
			else 
				break;
		}
		printf("%s\n", recvline);
	}
	exit(0);
	return 0;
}

