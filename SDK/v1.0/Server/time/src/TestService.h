



#pragma once 

#include "ObjService.h"
#include "Timer.h"

typedef struct tagTestInfo 
{
	char szName[128];
	char szEmail[128];

	long long llID;
	long long age;
	long long Teet;
}TESTINFO, *PTESTINFO;

typedef struct tagInfo 
{
	char szName[128];
	char szEmail[128];

	int  No; 
	long long llID;
	long long age;
	long long Teet;
}INFO, *PINFO;


class CTestService:
	public CObjService,
	public CTimer 
{
public:
	CTestService();
	virtual ~CTestService();
public:
	virtual bool	OnInitialUpdate();

	virtual bool	OnTimeOut(struct tm* pTime);

	virtual bool	OnSecondIdle();

	virtual void	OnTerminate();
	
	virtual bool	OnTimer(PTIMERHEADER  pTimer);
public:
	bool	OnPrintTimer(PTIMERHEADER pTimer);
private:

	std::vector<PINFO>				m_listval;
	std::vector<PTESTINFO>			m_listInfo;
	int								m_nFlag;
};

