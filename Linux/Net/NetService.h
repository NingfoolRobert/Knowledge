#pragma once
#include "lock.h"

#include "ThreadPool.h"
#include "NetClient.h"
#include <map>
#include <set>
#include <vector>

class CNetService
{
public:
	CNetService();
	virtual ~CNetService();
public:
	virtual bool OnInitialUpdate();
	virtual bool OnTimeOut(struct tm* pTime);
	virtual bool OnSecondIdle();
	virtual bool OnInvokeTerminate();
	virtual bool OnTerminate();
	
	virtual CNetClient*	CreateNetClient();
	
	virtual bool OnNetConnect(CNetClient* pNetClient);
	virtual bool OnNetBreak(CNetClient* pNetClient);
	virtual bool OnNetTickCount(CNetClient* pNetClient);
public:
	bool OnAccept();
	bool Accept(int fd, int nEnterPort, bool bEpoll = true);

public:
	bool CheckActive();
	static	bool CheckActiveSocket(CNetService* pService);
	
public:
	bool  AddClient(CNetClient* pNetClient);
	bool  DelClient(CNetClient* pNetClient);
private:
	CObjectLock							m_clsLock;
	std::map<int, CNetClient*>			m_listNetClient;
	
	CSocket								m_fdListen;
	Cond_Lock							m_condAccept;
//	std::vector<std::thread>			m_listThread;
	ThreadPool							m_ThreadPool;
};
