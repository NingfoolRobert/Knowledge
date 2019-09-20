#pragma once

#include "Socket.h"
#include <sys/epoll.h>

#include <unorder_map>
#include <unorder_set>



class CEpoll
{
public:
	CEpoll();
	virtual ~CEpoll();
public:
	virtual bool PollIn() = 0;
	virtual bool PollOut() = 0;

public:
	void	LoadEvent();
	void	UpdateEvent();
	void	UpdateEventMap();

	void	Poll(int timeout);
	bool	Add(CSocket* pSocket);
	bool	Del(int fd);

protected:
	
private:
	int m_fd;		//epoll_create 创建的套接字

	shared_ptr<CSocket>		m_HostSock;

	std::unordered_map<int,shared_ptr<CSocket> > m_listUserObj;
	std::unordered_set<shared_ptr<CSocket> > 
};
