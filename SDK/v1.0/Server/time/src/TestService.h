



#pragma once 

#include "ObjService.h"
#include "Timer.h"


class CTestService:
	public CObjService,
	public CTimer 
{
public:
	CTestService();
	virtual ~CTestService();
public:
	virtual bool	OnInitialUpdate();

	virtual bool	OnTimeOut(struct tm* pTime);

	virtual bool	OnSecondIdle();

	virtual void	OnTerminate();
	
	virtual bool	OnTimer(PTIMERHEADER  pTimer);
public:
	bool	OnPrintTimer(PTIMERHEADER pTimer);
};

