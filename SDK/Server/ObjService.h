/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxObjService.h
 * @brief:xxxObjService 
 * @author:xxxObjServicenbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2019-12-28
 */

#pragma once 

#include "ObjectService.h"
#include "Service.h"

class CObjService: public CObjectService,
	public CService 
{
public:
	CObjService(void);
	virtual ~CObjService(void);
public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual void OnTerminate();
};
