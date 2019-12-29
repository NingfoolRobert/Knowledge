#include "Thread.h"


static void* ActiveThread(void* param)
{
	CThread* pThread = static_cast<CThread*>(param);
	if(pThread == nullptr)
	{
		return nullptr;
	}
	pThread->ThreadProc();
	return nullptr;
}

CThread::CThread(Runnable* pTarget/* = nullptr*/):m_pTarget(pTarget), m_tid(0)
{
	
}


CThread::~CThread()
{

}

bool CThread::Start(int nMod/* = RUNMODE_DETACH*/)
{
	if(nMod >= RUNMODE_COUNT || nMod < 0)
		return false;

	int nRet = pthread_create(&m_tid, nullptr, &ActiveThread, this);

	if(nRet < 0)
		return false;

	if(nMod == RUNMODE_DETACH && Detach())
		return true;
	if(nMod == RUNMODE_JOIN && Join());
		return true;
	
	return false;
}

bool CThread::Join()
{
	if(m_tid == 0)
		return false;

	void *tRet ;
	int nRet = pthread_join(m_tid, &tRet);
	if(nRet != 0)
		return  false;
	return true;
}

bool CThread::Detach()
{
	if(m_tid == 0)
		return false;
	int nRet = pthread_detach(m_tid);
	if(nRet != 0)
		return false;
	
	return true;

}
	
unsigned int  CThread::ThreadProc()
{
	
	if(nullptr == m_pTarget)
	{
		Run();
	}
	else 
	{
		m_pTarget->Run();
	}
	return 0;
}
