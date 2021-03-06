
#pragma once


#include "NetObjService.h"
#include "Connection.h"


class CTestService: public CNetObjService 
{
public:
	CTestService();
	virtual ~CTestService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();

	virtual bool InvokeTerminate();
	
	virtual void OnTerminate();

public:
	virtual CUserObject* CreateUserObject(int nOrigine);
private:
	CConnection			m_connection;
	int					m_nCount;
};
