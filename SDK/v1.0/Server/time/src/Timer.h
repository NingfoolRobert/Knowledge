/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxTimer.h
 * @brief:xxxTimer 
 * @author:xxxTimernbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-04-24
 */

#pragma once 

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <vector> 
#include <queue> 
#include <mutex> 

#include "Buffer.h"
typedef struct tagTimerHeader 
{
	unsigned int	dwTimerID;
	unsigned int	dwLength;
	long long		tExpire;
}TIMERHEADER, *PTIMERHEADER;


struct  compTimer 
{
	bool operator()(CBuffer* arg1, CBuffer* arg2)
	{
		PTIMERHEADER pTimer1 = (PTIMERHEADER) arg1->GetBufPtr();
		PTIMERHEADER pTimer2 = (PTIMERHEADER) arg2->GetBufPtr();
	
		if(pTimer1->tExpire == pTimer2->tExpire)
		{
			return pTimer1->dwTimerID > pTimer2->dwTimerID;
		}

		return pTimer1->tExpire > pTimer2->tExpire;
	}
};

class CTimer 
{
public:
	CTimer();
	virtual ~CTimer();
public:
	virtual bool OnTimer(PTIMERHEADER pTimer) = 0;
	
	bool SetTimer(PTIMERHEADER pTimer, int nSec);

	bool SetTimerMilli(PTIMERHEADER pTimer, int nMilliSec);

	bool KillTimer(unsigned int dwTimerID);
public:
	void ActiveTimerFunc();
private:
	bool								m_bEnableTimer;
	bool								m_bStop;
	std::mutex							m_clsLock;
	std::priority_queue<CBuffer*, std::vector<CBuffer*>, compTimer>		m_listTimer;
};

