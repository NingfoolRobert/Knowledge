#include "NetObjService.h"


class CTestService: public CNetObjService 
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
	
		
};
