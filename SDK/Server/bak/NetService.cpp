#include "NetService.h"
#include "AutoLock.h"


#include <sys/epoll.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <thread> 
#include <algorithm> 


CNetService::CNetService(void):m_pAcceptIO(nullptr),m_nTimeOut(100),m_bStop(false)
{

}

CNetService::~CNetService()
{

}

bool CNetService::OnInitualUpdate()
{
	m_pAcceptIO = new CAcceptIO;
	if(nullptr == m_pAcceptIO)
	{
		LogError("%s(%d) Init Net AcceptIO fail.", __FILE__, __LINE__);
		return false;
	}
	if(!m_pAcceptIO->InitialUpdate(m_nPort, this))
	{
		return false;
	}
	
	
	m_ep = epoll_create(1000);
	if(m_ep <= 0)
	{
		LogError("%s(%d) Create epoll fd  fail.", __FILE__, __LINE__);
		return false;
	}

	

	std::thread tr1(&CNetService::ActiveEpollThread, this);
	tr1.detach();
	
	for(int i = 0; i < WORKFUNC_THREAD_COUNT; ++i)
	{
		std::thread tr(&CNetService::ActiveWorkFuncThread, this);
		tr.detach();
	}

	LogInfo("Start Net Service Success...");
    return true;
}
    
bool CNetService::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}

	LogInfo("Connection Count: %d, Add: %d.", m_mapClient.size(), m_listAddClient.size());
	return true;
}

bool CNetService::OnSecondeIdle()
{
	CAutoLock locker(&m_clsEpollLock);
	CNetClient* pNetClient = nullptr;
	for(auto it = m_mapClient.begin(); it != m_mapClient.end(); ++it)
	{
		pNetClient = it->second;
		if(pNetClient)
		{
			pNetClient->OnSecondIdle();
		}
	}
	return true;
}

void CNetService::OnTerminate()
{
	m_bStop = true;
	CNetClient* pNetClient = nullptr;
	CAutoLock locker(&m_clsEpollLock);
	
	for(auto it = m_mapClient.begin(); it != m_mapClient.end(); ++it)
	{
		pNetClient = it->second;
		if(nullptr == pNetClient)
			continue;
		pNetClient->Terminate();		
	}
	
}

void CNetService::OnNetConnect(int& fd)     //网络链接上来
{
    CNetClient* pNetClient = new CNetClient;
    if(nullptr == pNetClient)
    {
        LogError("%s(%d) memmory error.", __FILE__, __LINE__);
        return ;
    }
    //
    if(!pNetClient->Init(fd, this))
    {
        LogError("%s(%d) NetClient Init fail.", __FILE__, __LINE__);
        return ;
    }
    //
    if(!AddClient(pNetClient))
    {
        LogError("%s(%d) Add Client fail.");
    }
}

void CNetService::OnNetMsg(CNetClient* pNetClient, PHEADER pHeader) //网络消息到达
{
    if(nullptr == pNetClient || nullptr == pHeader)
    {
        LogError("%s(%d) init CNetClient is nullptr or PHEADER is nullptr.");
        return ;
    }
    
     pNetClient->OnMsg(pHeader);
}



void CNetService::ActiveWorkFuncThread()
{
    CNetClient* pNetClient = nullptr;
    while(true)
    {
        std::unique_lock<std::mutex>    locker(m_clsWorkLock);
        m_condWork.wait(locker, [&]()->bool{return !m_listWorkClient.empty();});
        locker.unlock();
        m_clsWorkLock.lock();
        if(!m_listWorkClient.empty())
        {
            pNetClient = m_listWorkClient.front();
            m_listWorkClient.pop();
        }
        m_clsWorkLock.unlock();
        //
        pNetClient->ActiveRecvThread();
    }
}
    

CUserObject*  CNetService::OnNetUserObject(PHEADER pHeader)
{
    return nullptr;
}


void CNetService::ActiveEpollThread()
{
    while(!m_bStop)
    {
        UpdateEvent();
        FdSockList    EventList;
        int nMax = m_mapClient.size();
        EventList.resize(nMax);
        int nCount = epoll_wait(m_ep, &EventList[0], nMax, m_nTimeOut);
        //
        if(nCount > 0)
            for_each(EventList.begin(), EventList.end(), std::bind(&CNetService::TriggerEvent, this, std::placeholders::_1));
    }
}

void CNetService::TriggerEvent(const struct epoll_event& ev)
{
    CNetClient* pNetClient  = (CNetClient*) ev.data.ptr;
	if(ev.events & (EPOLLIN | EPOLLERR | EPOLLHUP))
	{
		pNetClient->OnRecv();
		std::unique_lock<std::mutex> locker(m_clsWorkLock);
		m_listWorkClient.push(pNetClient);
		m_condWork.notify_one();
	}
	if(ev.events & EPOLLOUT)
		pNetClient->OnSend();
}

void CNetService::UpdateEvent()
{
    CNetClient* pClient = nullptr;
    while(true)
    {
        //
        m_clsAddClientLock.Lock();
        if(m_listAddClient.empty())
        {
           m_clsAddClientLock.UnLock();
           break;
        }
        pClient = m_listAddClient.front();
        m_listAddClient.pop();
        m_clsAddClientLock.UnLock();
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
        ev.data.fd = pClient->Detach();
        ev.data.ptr = pClient;
        epoll_ctl(m_ep, EPOLL_CTL_ADD, ev.data.fd, &ev);
    }

    while(true)
    {
        //
        m_clsDelClientLock.Lock();
        if(m_listDelClient.empty())
        {
           m_clsDelClientLock.UnLock();
           break;
        }
        pClient = m_listDelClient.front();
        m_listAddClient.pop();
        m_clsDelClientLock.UnLock();
        struct epoll_event ev;
        epoll_ctl(m_ep, EPOLL_CTL_DEL, ev.data.fd, &ev);
        
        pClient->OnBreak();
        delete pClient;
        pClient = nullptr;
    }
}
    
bool CNetService::AddClient(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
	{
		return false;
	}

	CAutoLock locker(&m_clsAddClientLock);
	m_listAddClient.push(pNetClient);
	return true;
}

bool CNetService::DelClient(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
	{
		return true;
	}

	CAutoLock locker(&m_clsDelClientLock);
	m_listDelClient.push(pNetClient);
	return true;
}
