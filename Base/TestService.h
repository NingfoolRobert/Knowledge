/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		TestService.h
 * @brief:		样例服务
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-28
 */


#pragma once

#include "NetObjService.h"

class CTestService :public CNetObjService
{
public:
	CTestService();
	virtual ~CTestService();
public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual bool InvokeTerminate();

	virtual bool OnTerminate();
	
	virtual bool OnRegisterObject();

	virtual CUserObject* OnCreateUserObject(int nOrigine);

};
