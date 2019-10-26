// 
// Call Center On Demand Product Series
// Copyright (C) 2019 ChannelSoft(Beijing.) Technology Ltd., Co.
// All right reserved
// 
// $$
// TODO:
// 
// *******************************************************/

#pragma once

#include "NetService.h"
#include "Service.h"
#include "UserObject.h"
#include "ObjectService.h"

class CNetClient;

class CNetObjService: 
			public CNetService, 
			public CObjectService,
			public CService 
{
public:
	CNetObjService();

	virtual ~CNetObjService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnServiceBegin();
	
	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();

	virtual bool OnTerminate();

public:
	virtual bool OnNetConnect(CNetClient* pNetClient);

	virtual bool OnNetMsg(CNetClient* pNetClient, PHEADER pMsg);
	
	virtual bool OnNetBreak(CNetClient* pNetClient);
	
	virtual bool OnNetTickCount(CNetClient* pNetClient);
	
public:
	virtual bool OnRegisterObject();

	virtual CUserObject* OnCreateUserObject(int nOrigine);


};

