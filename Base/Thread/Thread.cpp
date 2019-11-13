#include "Thread.h"
CThread::CThread()
{
	m_fnc = nullptr;
	m_pArgs = nullptr;
	m_pAttr = nullptr;
	m_tid = 0;
}

CThread::~CThread()
{

}

bool CThread::Init(PTHREADFUNC fnc,void* pArgs/* = nullptr*/, pthread_attr_t* pAttr/* = nullptr*/)
{
	if(fnc == nullptr)
		return false;

	m_fnc = fnc;
	m_pArgs = pArgs;
	m_pAttr = pAttr;

	return true;
}

bool CThread::Run(int nMod/* = RUNMODE_DETACH*/)
{
	if(nMod >= RUNMODE_COUNT || nMod < 0)
		return false;

	int nRet = pthread_create(&m_tid, m_pAttr, m_fnc, m_pArgs);

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
