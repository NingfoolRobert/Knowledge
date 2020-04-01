

#pragma once 
#include "NetClient.h"
#include "UserObject.h"
#include "AcceptIO.h"
#include "LogFile.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <set> 
#include <condition_variable> 


#define WORKFUNC_THREAD_COUNT 1


struct compNetClient
{
  bool operator()(const CNetClient* pNetClient1, const CNetClient* pNetClient2)
  {
		if(pNetClient1->m_dwIP == pNetClient2->m_dwIP)
		{
			return pNetClient1->m_nPort < pNetClient2->m_nPort;
		}
		
		return pNetClient1->m_dwIP < pNetClient2->m_dwIP;
  }  
};


class CNetService
{
public:
    CNetService(void);
    virtual ~CNetService();
public:
    virtual bool OnInitualUpdate();
    
    virtual bool OnTimeOut(struct tm* pTime);

    virtual bool OnSecondeIdle();

    virtual void OnTerminate();
public:
    virtual void OnNetConnect(int& fd);     //网络链接达到

    virtual void OnNetMsg(CNetClient* pNetClient, PHEADER pHeader); //网络消息到达
   
	virtual void OnNetMsg(CNetClient* pNetClient);					//网络消息到达

	virtual void OnNetBreak(CNetClient* pNetClient);				//网络断开

    virtual CUserObject* OnNetUserObject(PHEADER pHeader);
public:
    void ActiveWorkFuncThread();

public:
    CNetClient*     CreateNetClient(int fdSock, CNetService* pNetService);
    
    void ActiveEpollThread();
public:

    bool    AddClient(CNetClient* pNetClient);

    bool    DelClient(CNetClient* pNetClient);
private:
    void    TriggerEvent(const struct epoll_event& ev);
    
    void    UpdateEvent();

	void	LoadEvent(std::unordered_map<int, CNetIO*>::value_type &value);
public:
    int                             m_nPort;
private:
    CAcceptIO*                      m_pAcceptIO;
private:
    std::mutex                      m_clsWorkLock;
    std::condition_variable         m_condWork;
    std::queue<CNetClient*>         m_listWorkClient;
	std::queue<CNetClient*>			m_listDelClientS;
private:
    typedef std::vector<struct epoll_event>                            FdSockList;
    typedef std::unordered_map<int, CNetIO*>	                       FdNetIOMap;
    typedef std::set<CNetClient*, compNetClient>                       FdClientSet;

	
	CObjectLock						m_clsEpollLock;    
    FdNetIOMap						m_mapNetIO;
    FdClientSet                     m_listClient;
	FdNetIOMap						m_listModIO;
private:
    int                             m_nTimeOut;                     //
    int                             m_ep;                           //epoll 文件句柄
private:
	bool							m_bStop;
};
