#include "mmsock.h"

struct mdata
{
	int fa;
	unsigned int len;
	char buf[2048];
	mdata()
	{
	}
	mdata(char* s,const int len)
	{
		for(int i = 0; i < len; ++i)
			buf[i]=s[i];
	}
};

class mepoll
{
public:

	mepoll(unsigned short eventsnum = 20)
	{
		epfd = epoll_create(0xfff);
		maxevents = eventsnum;
		events = new epoll_event[maxevents];
		timeout = 1;
	}
	int add(int fd)
	{
		fcntl(fd, F_SETFL, O_NONBLOCK);
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = fd;
		return epll_ctl(epfd,EPOLL_CTL_ADD, fd, &ev);
		
	}
	void ctl_in(int index)
	{
		ev.data.fd = *(int*)events[index].data.ptr;
		ev.events = EPOLLIN | EPOLLET;
		epoll_ctl(epfd, EPOLL_CTL_MOD, *(int*)events[index].data.ptr,&ev);
	}
	void ctl_out(int index, mdata* data)
	{
		data->fd = events[index].data.fd;
		ev.data.ptr = data;
		ev.events = EPOLLOUT | EPOLLET;
		epoll_ctl(epfd, EPOLL_CTL_MOD, events[index].data.fd, &ev);
	}
	int wait()
	{
		return epoll_wait(epfd, events, maxevents, timeout);
	}

	unsigned int geteventype(int index)
	{
		return events[index].events;
	}
	msock getsock(int index)
	{
		msock  sk;
		sk.setsock(events[index].data.fd);
		return sk;
	}
	msock getsock(mdata* data)
	{
		msock sk;
		sk.setsock(data->fd );
		return sk;
	}
	mdata* getdata(int index)
	{
		return (mdata*)evens[index].data.ptr;
	}

public:
	int epfd;
	epoll_event ev, *events;
	int maxevents;
	int timeout;
	

};
