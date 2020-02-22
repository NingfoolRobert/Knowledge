#pragma once 


#include "NetService.h"
#include "ObjectService.h"
#include "Service.h"
#include "GenericFactory.h"


class CNetObjService:	public CNetService, 
						public CObjectService, 
						public CService 
{
public:
	CNetObjService(void);
	virtual ~CNetObjService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual bool InvokeTerminate();
	
	virtual void OnTerminate();
public:
    virtual CUserObject* OnNetUserObject(PHEADER pHeader);
	
	virtual CUserObject*	CreateUserObject(int nOrigine) = 0;
};
