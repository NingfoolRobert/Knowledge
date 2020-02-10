#include "AcceptIO.h"
#include "NetService.h"
#include "LogFile.h"

CAcceptIO::CAcceptIO(void):m_pNetService(nullptr)
{
}

CAcceptIO::~CAcceptIO()
{
}

bool CAcceptIO::InitialUpdate(int nPort, CNetService* pNetService)
{

    m_nPort = nPort;
    m_pNetService = pNetService;
    //
    if(Create() == 0)
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

    
    return true;
}


void CAcceptIO::ActiveAcceptThread()
{
    while(true)
    {
        int fd = Accept();
        if(fd <= 0)
        {
            break;
        }
        m_pNetService->OnNetConnect(fd);
    }
}

void CAcceptIO::StartListen(int nTimeOut /*= 1000*/)
{
    
}