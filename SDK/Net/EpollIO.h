/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		EpollIO.h
 * @brief:		EpollIO 
 * @author:		nbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2020-01-04
 */

#pragma once 

#include "Socket.h"


class CEpollIO 
{
public:
	CEpollIO(void);
	virtual ~CEpollIO(void);
public:
	

	
private:
	int			m_fEpoll;
};
