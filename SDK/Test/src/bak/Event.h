/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxEvent.h
 * @brief:xxxEvent 
 * @author:xxxEventnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-01-05
 */


#pragma once 

#include <sys/epoll.h>


class CEvent
{
	public:
		CEvent(void);
		~CEvent(void);
	public:
		void WaitForEvent(int * pEvent, int cnFd);
		
		void ActiveFuncThread();	
	private:
		int ep;
		int _event;
		int _etimer;
};
