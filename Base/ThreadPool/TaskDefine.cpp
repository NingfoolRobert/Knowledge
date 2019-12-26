#include "TaskDefine.h"
#include "ActiveObject.h"

CTimer::CTimer(CActiveObject* pObject, CBuffer* pBuf)
{
		m_pObject = pObject;
		m_pBuf = pBuf;
}
	
void CTimer::Run()
{
	if(m_pBuf == nullptr)
	{
		return ;
	}
	PTIMERHEADER pTimer = (PTIMERHEADER)m_pBuf->GetBufPtr();

	m_pObject->OnTimer(pTimer);
}

CEvent::CEvent(CActiveObject* pObj, CBuffer* pBuf)
{
	m_pObject = pObj;
	m_pBuf = pBuf;
}

void CEvent::Run()
{
	if(m_pObject == nullptr)
	{
		return ;
	}
	PEVENTHEADER pEvent = (PEVENTHEADER)m_pBuf->GetBufPtr();
	m_pObject->OnEvent(pEvent);
}
