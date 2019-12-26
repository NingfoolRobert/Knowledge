
#pragma once 

#include "ActiveObject.h"


class CObjectService:public CActiveObject 
{
public:
	CObjectService(void);
	virtual ~CObjectService(void);
public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeOut(struct tm *pTime);
	
	virtual bool OnSecondIdle();

	virtual void OnTerminate();
public:
	

};
