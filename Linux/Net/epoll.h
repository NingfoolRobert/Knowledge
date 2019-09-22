#pragma once

#include "Socket.h"
#include <sys/epoll.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "AutoLock.h"

#include "ThreadPool.h"



class CNetService;

class CNetClient;

class CEpoll
{
public:
	typedef std::unordered_map<int, CSocket*> IOMAP;
	CEpoll();
//	explicit CEpoll();
	virtual ~CEpoll();
public:
	bool  OnInitialUpdate(CNetService* pService, int cnThread);
	bool  AddClient(CNetClient* pNetClient);
public:
	virtual bool PollIn() = 0;
	virtual bool PollOut() = 0;

public:
	void	LoadEvent();
	void	UpdateEvent();
	void	UpdateEventMap();
	void	TriggerEvent();
	void	Poll(int timeout);
	bool	Add(CSocket* pSocket);
	bool	Del(int fd);
public:
	bool	Attach(CSocket* pSocket);
	bool	Detach(int fd);
protected:

private:
	CNetService*		m_pService;
private:
	int m_fd;		//epoll_create 创建的套接字
	
	int m_event;
	int m_newevent;
	bool	m_bUpdate;
	
	std::vector<struct epoll_event> m_events;
	
	int m_ep;

	IOMAP	m_listNetClient;
	IOMAP	m_newNetClient;
	
	CSocket*							m_HostSock;
	std::unordered_map<int, CSocket*>   m_listUserObj;
	//std::unordered_set<CSocket*,greater<int> >		m_listSocket;
	std::vector<int>					m_listFD;

	ThreadPool							m_ThreadPoll;

};
