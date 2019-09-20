#pragma once


#include "lock.h"

class CAutoLock
{

public:
	explicit CAutoLock(CObjectLock* pLock);
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
	CObjectLoc*		m_pLock;
}



