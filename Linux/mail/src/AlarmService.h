
#pragma once 
#include "Service.h"
//#include "CICCLog.h"
#include "Log.h"



class CAlarmService : public CService 
{
public:
	CAlarmService() = default;
	virtual ~CAlarmService() = default;

public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeout(struct tm* pTime);

	virtual bool OnSecondIdle();
	
	virtual bool InvokeTerminate();
	
	virtual bool OnTerminate();
	

public:
	
private:
	
};


