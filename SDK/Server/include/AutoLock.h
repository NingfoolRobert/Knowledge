/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxAutoLock.h
 * @brief:xxxAutoLock 
 * @author:xxxAutoLocknbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2019-12-28
 */

#pragma once 

#include "ObjectLock.h"


class CAutoLock
{
public:
    CAutoLock(CObjectLock* plock)
    {
		assert(plock);
		m_clsLock=plock;
		m_clsLock->Lock();
    }
    virtual ~CAutoLock()
    {
		m_clsLock->UnLock();
    }
private:
    CObjectLock*   m_clsLock;
};
