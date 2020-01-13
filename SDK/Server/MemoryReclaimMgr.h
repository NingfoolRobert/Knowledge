/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		MemoryStructReclaimMgr.h
 * @brief:		MemoryStructReclaimMgr 
 * @author:		nbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2020-01-13
 */


#pragma once  
#include <stdio.h>
#include <stdlib.h> 
#include <queue>
#include "AutoLock.h"


#define MALLOC_TYPE_MALLOC		1
#define MALLOC_TYPE_NEW			2

#define STRUCT_RECLAIM_MAX_COUNT		2000


template<class T>
class CMemoryReclaimMgr 
{
public:
	CMemoryReclaimMgr(void)
	{
		m_nMallocType = MALLOC_TYPE_MALLOC;
		m_nMaxEmpty = STRUCT_RECLAIM_MAX_COUNT;
	}

	CMemoryReclaimMgr(int nMallocType)
	{
		m_nMallocType = nMallocType;
		m_nMaxEmpty = STRUCT_RECLAIM_MAX_COUNT;
	}

	~CMemoryReclaimMgr(void)
	{
		ClearAll();
	}
public:

	void SetMallocType(int nMallocType)
	{
		m_nMallocType = nMallocType;
	}

	void SetMaxEmpty(int nMaxEmpty)
	{
		m_nMaxEmpty = nMaxEmpty;
	}

	T*	GetEmpty()
	{
		T* pObj = nullptr;
		CAutoLock locker(&m_clsLock);
		
		if(!m_listObject.empty())	
		{
			pObj = m_listObject.front();
			m_listObject.pop();
			return pObj;
		}
	
		m_nGetEmptyCount++;
		if(m_nMallocType == MALLOC_TYPE_NEW)
			return new T;
		
		if(m_nMallocType == MALLOC_TYPE_MALLOC)
			return (T*)malloc(sizeof(T));

		return pObj;
	}

	void ReclaimEmpty(T* pObj)
	{
		if(nullptr == pObj)
			return ;
		CAutoLock locker(&m_clsLock);
		if(m_nReclaimCount >= m_nMaxEmpty)
		{
			if(m_nMallocType == MALLOC_TYPE_NEW)
				delete pObj;
			if(m_nMallocType == MALLOC_TYPE_MALLOC)
				free(pObj);
			return ;
		}
		
		m_listObject.push(pObj);
		m_nReclaimCount++;
	}

	void ClearAll()
	{
		CAutoLock locker(&m_clsLock);
		T* pObj = nullptr;
		while(!m_listObject.empty())
		{
			pObj = m_listObject.front();
			m_listObject.pop();
			if(m_nMallocType == MALLOC_TYPE_NEW)
				delete pObj;
			if(m_nMallocType == MALLOC_TYPE_MALLOC)
				free(pObj);
			m_nReclaimCount--;
		}
	}
private:
	int						m_nMallocType;
	CObjectLock				m_clsLock;
	std::queue<T*>			m_listObject;
private:
	int						m_nGetEmptyCount;
	int						m_nReclaimCount;
	int						m_nMaxEmpty;
};
