//#include "NetObjService.h"
#include "Service.h"
#include <stdio.h>


class CTestService: public CService 
{
public:
	CTestService();
	virtual ~CTestService();
public:
	virtual bool OnInitialUpdate();
	virtual bool OnTimeout(struct tm* pTime);
	virtual bool OnSecondIdle();
	virtual bool InvokeTerminate();
	virtual bool OnTerminate();
	
public:
	
private:
	
	FILE* m_fp;
};
