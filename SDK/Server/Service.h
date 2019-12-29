#ifndef _SERVICE_H_
#define _SERVICE_H_ 

#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <mutex>
#include <condition_variable>



#include "Log.h"
#include "GlobalVar.h"




class CService
{
public:
	CService();
	virtual ~CService();
public:
	virtual bool  OnInitialUpdate() = 0;
	virtual bool  OnTimeOut(struct tm* pTime) = 0;
	virtual bool  OnSecondIdle() = 0;
	virtual bool  InvokeTerminate() = 0;
	virtual void  OnTerminate() = 0;

public:
	
	bool	Execute(const char* pszCommand);
	void	Terminate();

private:
	bool	SysRun();
protected:
	
	bool		m_bStop;
	time_t		m_tStart;
	std::mutex	m_clsLock;
	std::condition_variable  m_condStop;
};

extern CService* g_Service;

#endif //_SERVICE_H_ 
