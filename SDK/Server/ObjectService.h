/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxObjectService.h
 * @brief:xxxObjectService 
 * @author:xxxObjectServicenbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2019-12-26
 */

#pragma once 

#include "ActiveObject.h"


class CObjectService: public CActiveObject 
{
public:
	CObjectService(void);
	virtual ~CObjectService(void);
public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeOut(struct tm *pTime);
	
	virtual bool OnSecondIdle();

	virtual bool InvokeTerminate();

	virtual void OnTerminate();
public:
	

};
