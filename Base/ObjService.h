/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		ObjService.h
 * @brief:		CObjService
 * @author:		nbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2019-12-13
 */
#pragma once 


#include "ObjectService.h"
#include "Service.h"


class CObjService :
	public CObjectService,
	public CService 
{
public:
	CObjService();
	virtual ~CObjService();
public:
	virtual bool OnInitialUpdate();
	
	virtual bool OnTimeOut(struct tm* pTime);
	
	virtual bool OnSecondIdle();
	
	virtual bool OnTerminate();
	
	virtual bool OnMsg(PMSGHEADER pMsg);

	virtual bool OnRegisterObject();
};
