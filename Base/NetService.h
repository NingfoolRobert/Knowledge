
//#include <zmq.h>
//#include <czmq.h>

#include "NetClient.h"
#include "AcceptIO.h"
#include "AutoLock.h"
#include "UserObject.h"
#include "NetIOMgr.h" 



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
	
//	virtual CUserObject* CreateUserObject(int nType) = 0;

	virtual bool OnNetConnect(CNetClient* pNetClient);

	virtual bool OnNetMsg(CNetClient* pNetClient, PHEADER pMsg);
	
	virtual bool OnNetBreak(CNetClient* pNetClient);

	virtual bool OnNetTickCount(CNetClient* pNetClient);

public:

	bool AddNetClient(CNetClient* pNetClient);

	bool OnNetNetMsg(CNetClient* pNetClient, PHEADER pMsg);					//网络消息到达数据接口 
	
protected:

public:
	int			m_nPort;
	
protected:

	CAcceptIO*			m_pAcceptIO;

	CNetIOMgr*			m_pIOMgr;
};

