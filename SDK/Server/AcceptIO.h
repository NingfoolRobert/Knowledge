

#pragma once 

#include "Socket.h"

class CNetService;

class CAcceptIO:public CSocket
{
public: 
    CAcceptIO(void);
    virtual ~CAcceptIO();
public:
    bool InitialUpdate(int nPort, CNetService* pNetService);
    
public:
    void ActiveAcceptThread();

    void StartListen(int nTimeOut = 1000);
private:
    int             m_ep;               //epoll fd 
    int             m_nPort;            //
    CNetService*    m_pNetService;
};