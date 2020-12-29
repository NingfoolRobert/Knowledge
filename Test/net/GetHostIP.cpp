

#include <stdio.h> 
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <sys/ioctl.h> 
#include <sys/socket.h> 
#include <net/if.h> 
#include <unistd.h> 

int main()
{
	char szIP[16] = { 0 };

	int fd  = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd == -1)
	{
		printf("Acquire fail\n");
		return 0;
	}

	char buf[1024] = { 0 };
	struct ifreq *requ;
	struct ifconf ifc;
//	ifc.ifc_len = sizeof(requ);
//	ifc.ifc_buf = (caddr_t)&requ;
	ifc.ifc_len = 1024;
	ifc.ifc_buf = buf;
	
	if(!ioctl(fd, SIOCGIFCONF, (char*)&ifc))
	{
		if(0 == ifc.ifc_len / sizeof(struct ifreq))
		{
			close(fd);
			printf("Acquire faile\n");
			return 0;
		}
			requ = (struct ifreq*)ifc.ifc_buf;
//		if(!(ioctl(fd, SIOCGIFADDR, (char*)&requ)))
		{
			for(int i = (ifc.ifc_len /sizeof(struct ifreq)); i > 0; --i)
			{
				if(requ->ifr_flags == AF_INET)
				{
					printf("ethname=%s\n", requ->ifr_name);
					sprintf(szIP, "%s", inet_ntoa(((struct sockaddr_in*)(&requ->ifr_addr))->sin_addr));
					printf("%s\n", szIP);
				}
				++requ;
			}
			close(fd);
//			printf("%s\n", szIP);
			return 0;
		}
	}
	close(fd);
	return 0;
}
