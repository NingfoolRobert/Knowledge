
#pragma once

#include "ThreadPool.h"



class CObjectService
{
public:
	CObjectService();
	virtual ~CObjectService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();
	
	virtual bool OnTerminate();

public:
	virtual bool OnRegisterObject();
	
};

