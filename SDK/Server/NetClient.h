

#pragma once 

#include "Protocol.h"
#include "Buffer.h"
#include "ObjectLock.h"
#include "Socket.h"
#include "GFunc.h"

#include <queue>


class CUserObject;
class CNetService;

class CNetClient:   public CSocket 
{
public:
    CNetClient(void);
    virtual ~CNetClient();

public:
    bool   Init(int& fd);
public:
    virtual bool OnMsg(PHEADER pHeader);
    
    virtual void OnConnect();

    virtual bool OnSecondIdle();

    virtual void OnBreak();

public:
    virtual bool OnSend();
    
    virtual bool OnRecv();

    virtual bool OnClose();
public:
    
    void ActiveRecvThread();
public:
    bool SendMsg(PHEADER pMsg);
    
    bool SendZipMsg(PHEADER pMsg);

    bool SendMsg(CBuffer* pBuffer);

    bool Terminate();

public:
    void BindUserObject(CUserObject* pUserObj);
    
    bool IsBindUserObject();

    bool operator<(const CNetClient* pNetClient)
    {
        return m_fd < pNetClient->Detach();
    }
private:

    bool SendMsg(void* pBuf, int nLen);
private:
    CUserObject*            m_pUserObject;  
    CNetSerivce*            m_pNetService;
private:
    CObjectLock             m_clsSendLock;
    std::queue<CBuffer*>    m_listSendBuf;
    CBuffer*                m_pSendBuf;
    unsigned int            m_dwSendLen;
    
    CObjectLock             m_clsRecvLock;
    std::queue<CBuffer*>    m_listRecvBuf;
    CBuffer*                m_pRecvBuf;
    unsigned int            m_dwRecvLen;
private:
    unsigned long           m_ulSendSerialNum;      //发送序列号
    unsigned long           m_ulRecvSerialNum;      //接收序列号
protected:
    unsigned int            m_dwIP;                 //主机IP 
    int                     m_nPort;                
};