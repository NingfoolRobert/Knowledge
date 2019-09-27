#pragma once 

#include "ActiveObject.h"
#include "Service.h"

class CNetObjService:
	public CActiveObject, 
	public CService 
{
public:
	CNetObjService() =default;
	virtual ~CNetObjService() = default;
public:
	virtual bool OnInitialUpdate();
	virtual bool OnTimeout(struct tm* pTime);
	virtual bool OnSecondIdle();
	virtual bool InvokeTerminate();
	virtual bool OnTerminate();
public:
		

private:

};
