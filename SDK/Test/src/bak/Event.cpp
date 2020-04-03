#include "Event.h"
		


void CEvent::ActiveFuncThread()
{
	int fd[2] = { 0 };
	int nCount = epoll_wait(ep, &fd, 1, 1000);
	
	
}
