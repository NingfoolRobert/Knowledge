#include "NetService.h"

CNetService::CNetService(void):m_pAcceptIO(nullptr),m_nTimeOut(1000)
{

}

CNetService::~CNetService()
{

}
bool CNetService::OnInitualUpdate()
{
    
    return true;
}
    
bool CNetService::OnTimeOut(struct tm* pTime)
{

}

bool CNetService::OnSecondeIdle()
{

}

void CNetService::OnTerminate()
{

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
    
void CNetService::ActiveDelNetClientThread()
{
    CNetClient* pNetClient = nullptr;
    while(true)
    {
        m_clsDelClientLock.Lock();
        if(m_listDelClient.empty())
        {
            m_clsDelClientLock.UnLock();
            break;
        }
        pNetClient = m_listDelClient.front();
        m_listDelClient.pop();
        m_clsDelClientLock.UnLock();
        //TODO Delete NetClient;
    }
}

CUserObject*  CNetService::OnNetUserObject(PHEADER pHeader)
{
    return nullptr;
}


void CNetService::ActiveEpollThread()
{
    while(true)
    {
        UpdateEvent();
        FdClientList    EventList;
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
		pNetClient->OnRecv();
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
        
        pNetClient->OnBreak();
        delete pNetClient;
        pNetClient = nullptr;
    }
}