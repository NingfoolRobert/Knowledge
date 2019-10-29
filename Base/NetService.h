
//#include <zmq.h>
//#include <czmq.h>
#pragma once 
#include "NetClient.h"
#include "AutoLock.h"
#include "UserObject.h"
#include "BufferMgr.h"
#include "GFunc.h"
#include "ThreadPoolA.h"

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
	
	virtual bool OnNetConnect(CNetClient* pNetClient);					//网络链接到达端口

	virtual bool OnNetMsg(CNetClient* pNetClient, PHEADER pMsg);		//消息到达网络端口
	
	virtual bool OnNetBreak(CNetClient* pNetClient);					

	virtual bool OnNetTickCount(CNetClient* pNetClient);				//网络定时消息

public:

	bool AddNetClient(CNetClient* pNetClient);

public:
	int			m_nPort;
	
protected:

	CAcceptIO*			m_pAcceptIO;

	CIOMgr*				m_pIOMgr;
	
	CThreadPool*		m_pThreadPool;
};

