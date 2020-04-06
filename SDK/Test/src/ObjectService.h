/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		ObjService.h
 * @brief:		ObjService 
 * @author:		nbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2020-04-06
 */

#pragma once 

#include "ActiveObject.h"
#include "UserObject.h"


class CObjectService: 
		public CActiveObject 
{
public:
	CObjectService();
	virtual ~CObjectService();
public:
	virtual bool OnInitialUpdate();
	virtual bool OnTimeOut(struct tm* pTime);
	virtual bool OnSecondIdle();
	virtual bool OnTerminate();
public:
	virtual bool OnEvent(PEVENTHEADER pEvent);
public:
	virtual void RegisterObject();
	virtual CUserObject* OnCreateUserObject(int nOrigine) = 0;
public:
	CUserObject* CreateUserObject(int nOrigine);
};
