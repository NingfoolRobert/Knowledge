#include "AcceptIO.h"
#include "NetService.h"
#include "LogFile.h"

#include <thread>
#include <sys/epoll.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 


CAcceptIO::CAcceptIO(void):m_pNetService(nullptr)
{
	m_nEventType = EPOLLIN | EPOLLET;
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

	CSocket::SetBlockMode();
	LogInfo("Init Net Listen Success IP:Port = LocalIP:%d", m_nPort);
	
    return true;
}

bool CAcceptIO::OnRecv()
{
	while(true)
	{
		int fd = Accept();
   		if(fd <= 0)
   		{
			if(EINTR == errno)
			{
				continue;
			}
			else if(EAGAIN == errno)
			{
				break;
			}
			else 
			{
				LogError("%s(%d) Accpet NetClient fail. errno:%d", __FILE__, __LINE__, errno);
				break;
			}
		}
   		m_pNetService->OnNetConnect(fd);
	}
	return true;
}
	
bool CAcceptIO::Terminate()
{
	return false;
}

bool CAcceptIO::OnClose()
{
	return true;
}
