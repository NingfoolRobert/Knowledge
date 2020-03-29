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
		return false;
	}
	m_nPort = nPort;
    m_pNetService = pNetService;
	//
    if(Create() <= 0)
    {
        LogError("Create Socket fail.errcode: %d", errno);
        return false;
    }
	int bOn = 1;
	SetSockOpt(SOL_SOCK, SO_REUSEADDR, &bOn, sizeof(bOn));	
	//
    if(!Bind(nPort))
    {
        LogError("Bind Socket fail,errno:%d. LocalInternal:%d", errno, nPort);
        return false;
    }
    //
    if(!Listen())
    {
        LogError("Local Service Listen fail.errno:%d, Port:%d", errno, nPort);
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
				LogError("Accpet NetClient fail. errno:%d", errno);
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
