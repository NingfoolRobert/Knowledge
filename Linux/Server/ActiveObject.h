#pragma once

//#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <utility>
#include <functional>
#include <map>
#include <queue> 
//#include <priority_queue>




typedef struct stTimerHeader
{

	int						nTimerID;
	int						nLength;
	unsigned int			unInterSec;
	unsigned int			unInterUSec;
	unsigned int			unOwnerID;
	std::function<void()>	task;
	stTimerHeader(){
		nTimerID		= 0;
		unInterSec		= 0;
		unInterUSec		= 0;
		unOwnerID		= 0;
		nLength			= 0;
	}
	bool operator<(stTimerHeader& other)
	{
		if(unInterSec < other.unInterSec)
			return true;
		else if(unInterSec == other.unInterSec)
		{
			if(unInterUSec < other.unInterUSec)
				return true;
			else if(unInterUSec == other.unInterUSec )
			{
				if(unOwnerID < other.unOwnerID)
					return true;
				else if(unOwnerID == other.unOwnerID)
				{
					if(nTimerID < other.nTimerID)
						return true;
					else 
						return false;
				}
				return false;
			}
			else 
				return false;
		}
		else 
			return false;
	}
}TIMERHEADER, *PTIMERHEADER;
///////////////////////////////////////

class CActiveObject
{
public:
	CActiveObject();
	virtual ~CActiveObject();
public:

	bool  Init(int cnWorkThread);
public:
	virtual	bool OnTimeout(struct tm* pTime);
	virtual bool OnSecondIdle();
	virtual bool InvokeTerminate();
	virtual bool OnTerminate();
public:
	virtual bool SetTimer(PTIMERHEADER pTimer, int nSecond);
	
	virtual bool SetTimerMilli(PTIMERHEADER pTimer, int uSecond);
	
	virtual bool KillTimer(int nTimerID, void* pOwner);
public:
	void	TimerThread(CActiveObject* pObj);
private:
	bool			m_bStop;
		
	std::thread		m_TimerThread;
	std::priority_queue<TIMERHEADER> m_TimerTask;
};
