
#pragma once 

#include "NetObjService.h"
#include "GlobalVar.h"
#include "LogFile.h"
#include "Connection.h"
#include "AgentUser.h"

class CTestService:public CNetObjService 
{
public:
	CTestService(void);
	virtual ~CTestService(void);
public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();

	virtual void OnTerminate();
public:

	virtual bool OnEvent(PEVENTHEADER pEvent);
	
	virtual bool OnTimer(PTIMERHEADER pTimer);

	virtual void OnRegisterObject();

	virtual CUserObject* CreateUserObject(int nOrigine);

public:
	bool OnSendEvent(PEVENTHEADER pEvent);

	bool OnSendTimer(PTIMERHEADER pTimer);
private:


	CConnection			m_Connection;
};
