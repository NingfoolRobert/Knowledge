
//#include <zmq.h>
//#include <czmq.h>

#include "NetClient.h"
#include "AutoLock.h"
#include "UserObject.h"
//#include "NetIOMgr.h" 
#include "BufferMgr.h"

class CAcceptIO;
class CIOMgr;

class CNetService
{
//	friend class CNetClient;
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
	
	virtual bool OnNetConnect(CNetClient* pNetClient);

	virtual bool OnNetMsg(CNetClient* pNetClient, PHEADER pMsg);
	
	virtual bool OnNetBreak(CNetClient* pNetClient);

	virtual bool OnNetTickCount(CNetClient* pNetClient);

public:

	bool AddNetClient(CNetClient* pNetClient);

	bool OnNetNetMsg(CNetClient* pNetClient, PHEADER pMsg);					//网络消息到达数据接口 
	
public:
	int			m_nPort;
	
protected:

	CAcceptIO*			m_pAcceptIO;

	CIOMgr*			m_pIOMgr;
};

