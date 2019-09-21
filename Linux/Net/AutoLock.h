#pragma once


#include "lock.h"
#include <assert.h>

class CAutoLock
{

public:
	explicit CAutoLock(CObjectLock* pLock)
	{
		assert(pLock != nullptr);
		m_pLock = pLock;
		m_pLock->Lock();
	}
	~CAutoLock()
	{
		m_pLock->UnLock();
		m_pLock = nullptr;
	}
private:
		
	
private:
	CObjectLock*		m_pLock;
};



