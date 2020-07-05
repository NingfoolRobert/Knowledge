/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		ObjNetService.h
 * @brief:		ObjNetService 
 * @author:		ObjNetServicenbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2020-06-23
 */

#pragma once 

#include "NetService.h"


class CObjNetService: 
	public CNetService 
{
public:
	CObjNetService();
	virtual ~CObjNetService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual bool OnTerminate();
public:
	virtual bool OnNetMsg(CNetClient* pNetClient, PHEADER pHeader);
public:
	virtual CUserObject* OnCreateUserObject(int nType);
};
