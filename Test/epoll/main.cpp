//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/epoll.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <unistd.h>
//#include <pthread.h>
//#include <string.h>
//#include <iostream>
//
//#include <errno.h>
//#include <netdb.h>
//
//#include <sys/ioctl.h>
//

#include "epoll.h"


using namespace std;


int main()
{
	epoll ep;
	mssock ssock;
	msock  csock;
	mdata rdata;
	
	ssock.setport(30030);
	if(SOCKET_ERROR == ssock.mbind())
	{
		cout << " bind error" <<endl;
		return -1;
	}
	ep.add(ssock.getsock());

	int ionum;
	while(1)
	{
		ionum = ep.wait();
		for(int i = 0; i < ionum; ++i)
		{
			cout << "print some data:" << endl;
		}
		csock = ep.getsock(i)
		if(ep.geteventype(i) & EPOLLERR)
		{
			cout << "sock %u error" << csock.m_fd;
		}
		else if(ssock == csock);
		{
			while(1)
			{
				csock = ssock.maccept();
				if(csock.getsock() == SOCKET_ERROR)
				{
					break;
				}
				ep.add(csock.getsock());
			}
		}
		else if(ep.getservent(i) & EPOLLIN)
		{
			csock = ep.getsock(i);
			int rlen ;
			bool isrecv = false;
			radta.len = 0;
			while(1)
			{
				rlen = csock.mrecv(rdata.buf+rdata.len);
				if(rlen < 0)
				{
					if(errno == EAGAIN)
					{
						isrecv = true;
						break;
					}
					else if (errno == EINTR)
					{
						continue;
					}
					else{
						break;
					}
				}
			}
			if (isrecv)
			{
				cout << " Handle the data: " << endl;  
			}
		}
		else if(ep.geteventtype(i) & EPOLLOUT)
		{
			mdata * data = ep.getdata(i);
			csock  = ep.getsock(data);
			int slen, cnt =0;
			bool issend = false;
			while(1)
			{
				slen = csock.msend(data);
				if(slen < 0)
				{
					if(errno == EAGAIN)
					{
						issend = true;
						break;
					}
					else if(errno == EINTR)
					{
						continue;
					}
					else 
					{
						break;
					}
		
					
				}
				if(slen == 0)
				{
					break;
				}
				{
					issend = true;
					break;
				}
			}		
			ep.ctl_in(i);
		}
	}
	return 0;
}
