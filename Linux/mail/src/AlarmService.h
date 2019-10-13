
#pragma once 
#include "Service.h"
#include "NotifyMgr.h"

class CAlarmService : public CService 
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
	
public:
		
private:
	CNotifyMgr*		m_pNotify;	//通知管理类
	int				m_nPort;	//监听端口号 
};

extern class CAlarmService*	g_CIAlarmService;
