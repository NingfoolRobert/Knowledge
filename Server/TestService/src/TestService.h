/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxTestService.h
 * @brief:xxxTestService 
 * @author:xxxTestServicenbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-03-03
 */
#pragma once 


#include "NetObjService.h"



class CTestService: 
	public CNetObjService 
{
public:
	CTestService();
	virtual ~CTestService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual bool InvokeTerminate();

	virtual void OnTerminate();

	virtual void OnRegisterVersion();
public:
	
	virtual CUserObject* OnCreateUserObject(int nOrigine);

	virtual void OnRegisterObject();
};

