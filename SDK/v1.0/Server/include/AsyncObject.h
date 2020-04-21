/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxAsyncObject.h
 * @brief:xxxAsyncObject 
 * @author:xxxAsyncObjectnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-04-21
 */

#pragma once 
#include <stdio.h> 
#include <stdlib.h> 
#include "EventDef.h"
#include "Buffer.h"


class CAsyncObject 
{
public:
	CAsyncObject();
	virtual ~CAsyncObject();
public:
	void SetOwner(CAsyncObject* pOwner){m_pOwner = pOwner;}

	CAsyncObject* GetOwner(){return m_pOwner;}
public:

	virtual bool OnEvent(PEVENTHEADER pEvent);

	virtual bool PostEvent(PEVENTHEADER pEvent);

	virtual bool PostEvent(CBuffer* pBuffer);

private:
	CAsyncObject*			m_pOwner;

};
