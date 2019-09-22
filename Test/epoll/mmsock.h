#include "msock.h"


class mssock:public msock{
public:
	mssock()
	{
		createsock();
		
		newaddrlen = sizeof(newaddr);

	}
	int mbind()
	{
		return bind(m_fd, (struct sockadr*)&m_addr, sizeof(m_addr));
	}
	int mlisten(int num = 20)
	{
		return listen(m_fd, num);
	}
	msock maccept()
	{
		int newsock = accept(m_fd, (struct sockaddr*)&newadr, &newaddrlen);
		msock newmsock;
		newmsock.setsock(newsock);
	}
	
public:
	sockadr_in newaddr;
	socklen_t newaddrlen;
	
		
};
