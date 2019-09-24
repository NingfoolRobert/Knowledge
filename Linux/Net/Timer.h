#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>
#include <functional>


typedef struct stTimerHead
{
	unsigned int  unTimerID;
	unsigned int  unExpireSec;		//到期时间 秒
	unsigned int  unExpireUsec;		//到期时间 微妙
	void*		  pTimerOwner;
	bool operator ==(stTimerHead* pother)
	{
		if(unTimerID == pother->unTimerID && pTimerOwner == pother->pTimerOwner)
			return true;
		return false;
	}
}TIMERHEAD, *PTIMEHEAD;

class TimerI{
	public:
		virtual bool  OnTimer(unsigned int unTimerID,std::function<void*>& task);
};

class CTimer 
{
public:
	CTimer();
	virtual ~CTimer();
public:
	bool SetTimer(unsigned int unTimerID,unsigned int nSecond, std::function<void*> &task, void* pTimerOwner = nullptr);
	
	bool SetTimerMilli(unsigned int unTimerID, unsigned int uSecond, std::function<void* > &task, void* pTimerOwner = nullptr);

	bool KillTime(unsigned int unTimerID,void* pTimerOwner);
	
	void Update(struct timeval* pTv);
public:
	void Run();

	

private:
	struct tm m_tnow;
	bool	m_bRun;
	std::unordered_map<TIMERHEAD, std::function<void* > > m_listTimer;
	
};
