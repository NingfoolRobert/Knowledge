#pragma once 
#include "czmq.h"
#include "BaseHandler.h"
#include <string>


class CAppUser:
	public BaseHandler
{
public:

	CAppUser();
//	CAppUser(std::string g, std::string h, std::string t);
	virtual ~CAppUser();
public:

	virtual void ProcessItem(zmsg_t * zMsg);



		
};
