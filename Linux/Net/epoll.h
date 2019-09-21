#pragma once

#include "Socket.h"
#include <sys/epoll.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>



class CEpoll
{
public:
	CEpoll();
	explicit CEpoll();
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
public:
	bool	Attach(CSocket* pSocket);
	bool	Detach(int fd);
protected:
	
private:
	int m_fd;		//epoll_create 创建的套接字

//	shared_ptr<CSocket>		m_HostSock;
	CSocket*							m_HostSock;
	std::unordered_map<int, CSocket*>   m_listUserObj;
	//std::unordered_set<CSocket*,greater<int> >		m_listSocket;
	std::vector<int>					m_listFD;

};
