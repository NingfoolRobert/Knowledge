#include "AsyncObject.h"

CAsyncObject::CAsyncObject():m_pOwner(nullptr)
{

}

CAsyncObject::~CAsyncObject()
{
}

bool CAsyncObject::OnEvent(PEVENTHEADER pEvent)
{
	if(m_pOwner)
	{
		m_pOwner->OnEvent(pEvent);
	}
	
	return false;
}

bool CAsyncObject::PostEvent(PEVENTHEADER pEvent)
{
	return false;
}

bool CAsyncObject::PostEvent(CBuffer* pBuffer)
{
	return false;
}

bool CAsyncObject::OnTimer(PTIMEHEADER  pTimer)
{
	return false;
}

bool CAsyncObject::SetTimer(PTIMEHEADER pTimer, int nSecond)
{
	return false;
}

bool CAsyncObject::SetTimerMilli(PTIMEHEADER pTimer, int nMilliSec)
{
	return false;
}
	
bool CAsyncObject::KillTimer(unsigned int dwTimerID, int64_t pTimerOwner/* = 0*/)
{
	return false;
}
