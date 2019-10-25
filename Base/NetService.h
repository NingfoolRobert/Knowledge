
//#include <zmq.h>
//#include <czmq.h>

#include "NetClient.h"
#include "AcceptIO.h"
#include "AutoLock.h"
#include "UserObject.h"
#include "NetIOMgr.h" 
#include "AcceptIO.h"



class CNetService
{
	friend class CNetClient;
public:
	CNetService();

	virtual ~CNetService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();
	
	virtual bool OnTerminate();
	
public:
	virtual CNetClient* CreateNetClient();
	
	virtual CUserObject* CreateUserObject(int nType) = 0;
		
public:

	bool AddNetClient(CNetClient* pNetClient);

	//bool OnNetNetMsg(CNetClient* pNetClient)
protected:

public:
	int			m_nPort;
	
protected:

	CAcceptIO*			m_pAcceptIO;

	CNetIOMgr*			m_pIOMgr;
};

