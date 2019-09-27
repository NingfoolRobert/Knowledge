#pragma once

#include "ActiveObject.h"


class CObjectService:public CActiveObject
{
public:
	CObjectService() = default;
	virtual ~CObjectService() = default;
public:
	virtual bool OnInitialUpdate();
	virtual bool OnTimeOut(struct tm* pTime);
	virtual bool OnSecondIdle();
	virtual bool OnInvokeTerminate();
	virtual bool OnTerminate();

public:
	virtual bool PostEvent(std::function<void()> task);

private:
	

};
