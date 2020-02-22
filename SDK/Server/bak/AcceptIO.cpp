#include "AcceptIO.h"
#include "NetService.h"
#include "LogFile.h"

#include <thread>
#include <sys/epoll.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 


CAcceptIO::CAcceptIO(void):m_pNetService(nullptr)
{
}

CAcceptIO::~CAcceptIO()
{
}

bool CAcceptIO::InitialUpdate(int nPort, CNetService* pNetService)
{
	if(nPort < 1024) 
	{
		LogError("Net Listen Illegal Port. Port:%d ", nPort);
		return true;
	}
	m_nPort = nPort;
    m_pNetService = pNetService;
	m_ep = epoll_create(1000);
	if(m_ep <= 0)
	{
		LogError("%s(%d) Create epoll fd fail.", __FILE__, __LINE__);
		return false;
	}
	//
    if(Create() <= 0)
    {
        LogError("%s(%d) Create Socket fail.", __FILE__, __LINE__);
        return false;
    }
    //
    if(!Bind(nPort))
    {
        LogError("%s(%d) Bind Socket fail. LocalInternal:%d", __FILE__, __LINE__, nPort);
        return false;
    }
    //
    if(!Listen())
    {
        LogError("%s(%d) Local Service Listen fail. Port:%d", __FILE__, __LINE__, nPort);
        return false;
    }
	struct epoll_event ev;
	
	epoll_ctl(m_ep, EPOLL_CTL_ADD, Detach(), &ev);

	std::thread tr1(&CAcceptIO::ActiveAcceptThread, this);
	tr1.detach();
 
	sockaddr_in addrLocal;
	socklen_t nLen = sizeof(addrLocal);
	
	LogInfo("Net Listen Success IP:Port = LocalIP:%d", m_nPort);
	
    return true;
}

void CAcceptIO::ActiveAcceptThread()
{
	struct epoll_event event[4];
	while(!m_bStop)
    {
		int nCount = epoll_wait(m_ep, event, 4, 1000);
		if(nCount <= 0)
		{
			continue;
		}
		while(true)
		{
			int fd = Accept();
       		if(fd <= 0)
       		{
				LogError("%s(%d) Accpet NetClient fail. errno:%d", __FILE__, __LINE__);
				continue;
       		}
       		m_pNetService->OnNetConnect(fd);
		}
    }
}

