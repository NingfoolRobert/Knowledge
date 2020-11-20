

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <net/if.h>
#include <arpa/inet.h>
 
#define CLIENT_PORT 9000
#define GROUP "239.0.0.2"
 
int main(int argc, char **argv)
{
	struct sockaddr_in serveraddr;
	struct sockaddr_in localaddr;
	int confd;
	ssize_t len;

	char buf[128];
	struct ip_mreqn group;//组播结构体
	confd = socket(AF_INET,SOCK_DGRAM,0);//创建socket文件描述符
	bzero(&localaddr,sizeof(localaddr));//初始化结构体
	localaddr.sin_family = AF_INET;				//AF_INET
	inet_pton(AF_INET,"0.0.0.0",&localaddr.sin_addr.s_addr);
	localaddr.sin_port = htons(CLIENT_PORT);
	if(-1 == bind(confd,(struct sockaddr*)&localaddr,sizeof(localaddr)))
	{
		printf("bind success.\n");
	}
			
	inet_pton(AF_INET,GROUP,&group.imr_multiaddr);//设置组地址 
	inet_pton(AF_INET,"0.0.0.0",&group.imr_address);//本地任意IP
	group.imr_ifindex = if_nametoindex("ens33");
	setsockopt(confd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&group,sizeof(group));
	printf("Recv start... \n");
	
	while(1)
	{
		len = recvfrom(confd,buf,sizeof(buf),0,NULL,0);
	//	write(STDOUT_FILENO,buf,len);
		printf("%s", buf);
			
	}
	close(confd);
	return 0;
}
 
 
 
