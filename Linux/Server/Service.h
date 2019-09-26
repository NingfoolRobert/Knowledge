#ifndef _SERVICE_H_
#define _SERVICE_H_ 

#include <sys/time.h>
#include <unistd.h>
#include <time.h>


class CService
{
public:
	CService();
	virtual ~CService();
public:
	virtual bool  OnInitialUpdate() = 0;
	virtual bool  OnTimeout(struct tm* pTime) = 0;
	virtual bool  OnSecondIdle() = 0;
	virtual bool  InvokeTerminate() = 0;
	virtual bool  OnTerminate() = 0;

public:
	
	bool	Excute(const char* pszFileName = "", int nLine = 0);

private:
	bool		m_bStop;

	time_t		m_tStart;

};


extern CService* g_Service;

#endif //_SERVICE_H_ 
