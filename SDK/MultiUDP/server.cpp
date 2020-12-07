/* 
*broadcast_server.c - 多播服务程序 
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 




#define MCAST_PORT 8888  
#define MCAST_ADDR "224.0.0.88"			// /*一个局部连接多播地址，路由器不进行转发*/ 
#define MCAST_DATA "BROADCAST TEST DATA" // /*多播发送的数据*/ 
#define MCAST_INTERVAL 5 // /*发送间隔时间*/ 



int main(int argc, char**argv) 
{ 
    int s; 
    struct sockaddr_in mcast_addr; 
    s = socket(AF_INET, SOCK_DGRAM, 0); /*建立套接字*/ 
    if (s == -1) 
    { 
        perror("socket()"); 
        return -1; 
    } 
      
    memset(&mcast_addr, 0, sizeof(mcast_addr));/*初始化IP多播地址为0*/ 
    mcast_addr.sin_family = AF_INET; /*设置协议族类行为AF*/ 
    mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);/*设置多播IP地址*/ 
    mcast_addr.sin_port = htons(MCAST_PORT); /*设置多播端口*/ 
	ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = inet_addr("0.0.0.0");
	if(setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,sizeof(mreq)) < 0)
	{
		printf("add muliticast fail.\n");
		return 0;
	}

	unsigned char bMLoop = 1;
	if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, (void*)&bMLoop, sizeof(bMLoop)) < 0)
	{
		printf("set socket loop back fail.\n");
		return 0;
	}
                                                    /*向多播地址发送数据*/ 
	printf("send start ...\n");
	int num = 0;
    
	while(true) { 
		
		printf("send udp: %s. \n", MCAST_DATA);
        
		int n = sendto(s, /*套接字描述符*/ 
                                    MCAST_DATA, /*数据*/ 
                                    sizeof(MCAST_DATA), /*长度*/ 
                                    0, 
                                    (struct sockaddr*)&mcast_addr, 
                                    sizeof(mcast_addr)) ; 
		
        if( n < 0) 
        { 
            perror("sendto()"); 
            return -2; 
        }
		else 
		{
			printf("send udp muliticast: %s. num = %d\n", MCAST_DATA, num++);
		}
          
        sleep(MCAST_INTERVAL); /*等待一段时间*/ 
    } 
      
    return 0; 
}
