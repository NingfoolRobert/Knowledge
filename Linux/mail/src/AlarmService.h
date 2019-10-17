
#pragma once 



#include "NotifyMgr.h"
#include "../../include/Server/NetObjService.h"
#include "AppUser.h"


class CAlarmService : public CNetObjService 
{
public:
	CAlarmService();
	virtual ~CAlarmService();

public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeout(struct tm* pTime);

	virtual bool OnSecondIdle();
	
	virtual bool InvokeTerminate();
	
	virtual bool OnTerminate();

	virtual BaseHandler* CreateHandler(int nType);
	
public:
		
private:
	CNotifyMgr*		m_pNotify;	//通知管理类
};

extern class CAlarmService*	g_ciccAlarmService;
