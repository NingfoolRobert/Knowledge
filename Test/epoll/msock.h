#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
//#include <iostream>

#include <errno.h>
#include <netdb.h>

#include <sys/ioctl.h>

using namespace std;
class msock
{
public:

	msock()
	{
		memset(&m_addr, 0, sizeof(m_addr));
		m_addr.sin_family = AF_INET;
	}
	
	void setsock(int fd)
	{
		m_fd = fd;
	}
	int getsock()
	{
		return m_fd;
	}
	void CreateSock()
	{
		m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(m_fd < 0)
		{
			std::cout << "socket build error" << std::endl;
			exit(-1);
		}
	}
	void setioctl(bool bNonBlock)
	{
		fcntl(m_fd, F_SETFL, O_NONBLOCK);
	}
	bool setip(string ip)
	{
		hostent *hname = gethostbyname(ip.c_str());
		if(nullptr != hname)
		{
			std::cout << "Can't find address" << std::endl;
			return false;
		}
		m_addr.sin_addr.s_addr =*(unsigned int*)hname->h_addr_list[0];
		return true;
	}
	void setport(int port)
	{
		m_addr.sin_port = htons(port);
	}
	int msend(const char* data, const int len)
	{
		return send(m_fd, data, len, 0);
	}
	int msend(const string data)
	{
		return msend(data.c_str(), data.length());
	}
	int mrecv(char* data, int len)
	{
		return recv(m_fd, data, len, 0);
	}
	int mclose()
	{
		return close(m_fd);
	}
	int operator==(msock job)
	{
		return m_fd == job.getsock();
	}
public:
	int  m_fd;
	sockaddr_in m_addr;
};
