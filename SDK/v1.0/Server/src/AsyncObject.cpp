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
		return m_pOwner->OnEvent(pEvent);
	}

	return true;
}

bool CAsyncObject::PostEvent(PEVENTHEADER pEvent)
{
	return true;
}

bool CAsyncObject::PostEvent(CBuffer* pBuffer)
{
	return true;
}
