#pragma once 
#include "czmq.h"
#include "BaseHandler.h"
#include <string>
#include "AlarmService.h"
#include "PBMessageDefine.h"


class CAppUser:
	public BaseHandler
{
public:

	CAppUser();
	virtual ~CAppUser();
public:

	virtual void ProcessItem(zmsg_t * zMsg);


protected:
	
	bool ParseWarnningInfo(/*PB::Message* pMessage, */);
		
};
