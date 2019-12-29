
#pragma once 

#include "ObjService.h"



class CTestService:public CObjService 
{
public:
	CTestService(void);
	virtual ~CTestService(void);
public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();

	virtual void OnTerminate();

};
