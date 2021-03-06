/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		ObjectService.h
 * @brief:		CObjectService 
 * @author:		nbf,nbf0537@163.com
 * @version:	1.0
 * @date:	2019-12-13
 */

#pragma once

#include "AsyncObject.h"

class CObjectService: public CAsyncObject 
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
	virtual bool OnMsg(PMSGHEADER pAsyncMsg);

	virtual bool PostAsyncMsg(PMSGHEADER pMsg);

	virtual bool PostAsyncMsg(CBuffer* pBuffer);
public:
	virtual bool OnRegisterObject();
	
};
