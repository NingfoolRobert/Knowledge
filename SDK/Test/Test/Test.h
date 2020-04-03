
#pragma once 

#include "ActiveObject.h"
#include <atomic> 
#include <stdlib.h> 
#include <stdio.h> 
#include <mutex> 



typedef struct tagTestEvent
{
	EVENTHEADER header;
	long	tSec;
	long	tUsec;
	int64_t  tStat;
}TESTEVENT, *PTESTEVENT;



class CTest:public CActiveObject 
{
public:
	CTest();
	virtual ~CTest();
public:
	virtual bool OnInitialUpdate();

	virtual bool OnEvent(PEVENTHEADER pEvent);

public:

	void TestEvent();

	bool OnTestEvent(PEVENTHEADER pEvent);
	
	void PrintInfo();

	void TestAlloc();

	void TestLocker();
private:
	std::mutex					m_clsLock;
	std::atomic_long			m_llSum;
	std::atomic_long			m_nSend;
	std::atomic_long			m_nRecv;
};


