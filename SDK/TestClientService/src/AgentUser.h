#pragma once  


#include "UserObject.h"


class CAgentUser:public  CUserObject 
{
public:
	CAgentUser();
	virtual ~CAgentUser();
public:
	virtual bool OnMsg(PHEADER pHeader);
	
	virtual void OnConnect();
	
	virtual void OnSencondIdle();

	virtual void OnClose();


public:
	bool OnRecvData(PHEADER pHeader);

};

