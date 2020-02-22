

#pragma once 

#include "NetIO.h"

class CNetService;

class CAcceptIO:public CNetIO
{
public: 
    CAcceptIO(void);
    virtual ~CAcceptIO();
public:
    bool InitialUpdate(int nPort, CNetService* pNetService);

	bool Terminate();
public:
	virtual bool OnRecv();
	
	virtual bool OnClose();
private:
    int             m_nPort;            //
    CNetService*    m_pNetService;
};
