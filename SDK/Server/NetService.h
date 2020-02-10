

#pragma once 
#include "NetClient.h"
#include "UserObject.h"
#include "AcceptIO.h"
#include "LogFile.h"

#include <vector>
#include <map>
#include <unordered_map>

struct compNetClient
{
  bool operator()(const CNetClient* pNetClient1, const CNetClient* pNetClient2)
  {
      return pNetClient1->Detach() < pNetClient2->Detach();
  }  
};


class CNetService
{
public:
    CNetService(void);
    virtual ~CNetService();
public:
    virtual bool dfsa();
    
    virtual bool OnTimeOut(struct tm* pTime);

    virtual bool OnSecondeIdle();

    virtual void OnTerminate();
public:
    virtual void OnNetConnect(int& fd);     //网络链接达到

    virtual void OnNetMsg(CNetClient* pNetClient, PHEADER pHeader); //网络消息到达
    
    virtual CUserObject* OnNetUserObject(PHEADER pHeader);
public:
    void ActiveWorkFuncThread();
    
    void ActiveDelNetClientThread();
public:
    CNetClient*     CreateNetClient(int fdSock, CNetService* pNetService);

    
    void ActiveEpollThread();
public:

    bool    AddClient(CNetClient* pNetClient);

    bool    DelClient(CNetClient* pNetClient);
private:
    void    TriggerEvent(const struct epoll_event& ev);
    
    void    UpdateEvent();
public:
    int                             m_nPort;
private:
    CAcceptIO*                      m_pAcceptIO;
private:
    std::mutex                      m_clsWorkLock;
    std::condition_variable         m_condWork;
    std::queue<CNetClient*>         m_listWorkClient;

    CObjectLock                     m_clsAddClientLock;
    std::queue<CNetClient*>         m_listAddClient;
    CObjectLock                     m_clsDelClientLock;
    std::queue<CNetClient*>         m_listDelClient;
private:
    typedef std::vector<struct epoll_event>                            FdSockList;
    typedef std::unordered_map<int, CNetClient*>                       FdClientMap;
    typedef std::set<CNetClient*, compNetClient>                       FdClientSet;
    
    FdClientMap                     m_mapClient;
    FdClientSet                     m_listClient;
private:
    int                             m_nTimeOut;                     //
    int                             m_ep;                           //epoll 文件句柄
};