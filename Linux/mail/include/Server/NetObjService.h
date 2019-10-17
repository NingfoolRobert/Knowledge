#pragma once

#include "Service.h"
#include "SubService.h"
#include "GenericFactory.h"


class CNetObjService :public CService, public CSubService
{
public:
	CNetObjService();
	virtual ~CNetObjService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeout(struct tm* pTime);
	
	virtual bool OnSecondIdle();

	virtual bool InvokeTerminate();

	virtual bool OnTerminate();

	virtual BaseHandler* CreateHandler(int nType);
};
