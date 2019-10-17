#pragma once 

#include <czmq.h>
#include <map>
#include <time.h>
#include "SubObj.h"
#include "BaseHandler.h"

class CSubService
{
public:
	CSubService();
	virtual ~CSubService();
public:

	virtual bool OnInitualUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual bool InvokeTerminate();

	virtual bool OnTerminate();

	virtual BaseHandler* CreateHandler(int nType) = 0;

public:	
	bool LoadNetSettingsXMLConfig();

private:
	std::map<int, CSubObj*> m_mapSubObj;
	
};
