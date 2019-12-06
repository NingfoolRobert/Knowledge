/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		AutoLock.h
 * @brief:		AutoLock
 * @author:		AutoLocknbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2019-12-01
 */

#include "ObjectLock.h"
#include <assert.h> 

class CAutoLock
{

public:
	CAutoLock(CObjectLock* pObjectLock)
	{
		assert(pObjectLock);
		m_pObjectLock = pObjectLock;
		m_pObjectLock->Lock();
	}

	~CAutoLock(void)
	{
		m_pObjectLock->UnLock();
	}

private:
	CObjectLock*	m_pObjectLock;
};

