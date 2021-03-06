
#pragma once 

#include "Protocol.h"
#include "Buffer.h"
#include "ObjectLock.h"
#include "NetIO.h"
#include "GFunc.h"

#include <queue>


class CUserObject;
class CNetService;

class CNetClient:   
	public CNetIO 
{
public:
    CNetClient(void);
    virtual ~CNetClient();

public:
    bool   Init(int& fd, CNetService* pNetService);
public:
    virtual bool OnMsg(PHEADER pHeader);
    
	virtual void OnSecondIdle();
    
	virtual void OnConnect();

	virtual void OnBreak();
public:
    virtual bool OnSend();
    
    virtual bool OnRecv();
	
	virtual bool OnClose();
    
public:
    void ActiveRecvThread();
public:
    bool SendMsg(PHEADER pMsg);
    
    bool SendMsg(CBuffer* pBuffer);

    void Terminate();

public:
    void BindUserObject(CUserObject* pUserObj);
    
    bool IsBindUserObject();

private:

    bool SendMsg(const char* pBuf, int nLen);
private:
    CUserObject*            m_pUserObject;  
    CNetService*            m_pNetService;
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
    unsigned int           m_dwSendSerialNum;      //发送序列号
    unsigned int           m_dwRecvSerialNum;      //接收序列号 
public:
    unsigned int            m_dwIP;                 //主机IP 
    int                     m_nPort;               
private:
	unsigned int			m_dwOrigin;
};
