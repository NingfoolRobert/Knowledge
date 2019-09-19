#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>


class CEpollService
{
public:
	CEpollService();
	CEpollService(int ports,int thread):m_bStop(false),m_nThreadNum(thread),m_nPort(ports){

	}
	~CEpollService()
	{
	}
public:
	void init();
	void epoll();

	int setnonblocking(int fd)
	{
		int old_potion = fcntl(fd,F_GETFL);
		int new_potion = old_potion | O_NONBLOCK;
		fcntl(fd,F_SETFL,new_potion);
		return old_potion;
	}
	void addfd(int epollfd,int sockfd,bool oneshot)
	{
		epoll_event  event;
		event.data.fd = sockfd;
		event.events = EPOLLIN | EPOLLET;
		if(oneshot)
		{
			event.events |= EPOLLONSHOT;
		}
		epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&event);
		setnonblocking(sockfd);
	}
private:
	bool	m_bStop;

	int		m_nThreadNum;
	int		m_fdSocket;
	int		m_nPort;
	int		m_fdEpoll;
	epoll_event events[1024];
	struct sockaddr_in m_addr;
public:
	

};






void CEpollService::init()
{
	bzero(&m_addr,sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(m_nPort);
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	m_fdSocket = socket(AF_INET,SOCK_STREAM,0);
	if(m_fdSocket < 0)
	{
		assert(false);
		return ;
	}
	int nRet = bind(m_fdSocket, (struct sockaddr*)&m_addr,sizeof(m_addr));
	if(nRet < 0)
	{
		assert(false);
		return ;
	}
	nRet = listen(m_fdSocket,10);
	if(nRet < 0)
	{
		assert(false);
		return;
	}

	m_fdEpoll = epoll_create(1024);
	if(m_fdEpoll < 0)
	{

		assert(false);
		return;
	}
	//TODO 初始化线程池
}
void CEpollService::epoll()
{
	addfd(m_fdEpoll,m_fdSocket,false);
	while(!m_bStop)
	{
		int nRet = epoll_wait(m_fdEpoll,events,1024,-1);
		if(nRet < 0)
		{
			break;
		}
		for(int i =0 ;i <nRet ;++i)
		{
			int fd = events[i].data.fd;
			if(fd == m_fdSocket)
			{
				struct sockaddr_in clienAddr;
				socklen_t len = sizeof(clienAddr);
				int confd = accept(m_fdSocket,(struct sockadrr*)&clienAddr,&len);
				addfd(m_fdEpoll,confd,false);
			}
			else if(event[i].events & EPOLLIN)
			{
				char buf[1024];
				strset(buf,0);
				int ret = read(fd,buffer,1023);
				if(ret == 0)	//某个fd关闭了链接，从epoll中删除
				{
					struct epoll_event ev;
					ev.data.fd = fd;
					epoll_ctl(m_fdEpoll,EPOLL_CTL_DEL,&ev);
					shutdown(fd,SHUT_RDWR);
					continue;
				}
				else if(ret < 0)
				{
					//TODO 重新读取
				}
				else
				{
					//TODO 数据读取成功
					//
				}
			}
			else if(events[i].events & EPOLLOUT) // 可写
			{


			}
		}
	}
}





