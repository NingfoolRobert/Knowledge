

#pragma once 

#include "Protocol.h"
#include "Available.h"

class CNetClient;

class CUserObject: public CAvailable
{
public:
    CUserObject(void);
    virtual ~CUserObject();
public:
 
    virtual bool OnMsg(PHEADER pHeader);

    virtual void OnConnect();

    virtual void OnSecondIdle();
    
    virtual void OnClose();
public:

    bool SendMsg(PHEADER pHeader);

    bool SendZipMsg(PHEADER pHeader);

    void BindNetClient(CNetClient* pNetClient);
    
    bool IsBindNetClient();

public: 
    int                 m_nPort;
    unsigned int        m_dwIP;
private:
    CNetClient*        m_pNetClient;    
};
