/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		AsyncZMQMsgFunc.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-05-19
 */

#pragma once 

#include "ActiveObject.h"
#include "BufferMgr.h"
#include "Protocol.h"
#include <vector> 

template<class TYPE>
class CAsyncMsgFunc:public CActiveObject
{
public:
	typedef bool (TYPE::*PZMQMSGFUNC)(PZMQHEADER);
	CAsyncMsgFunc():m_pObj(nullptr),m_pfn(nullptr), m_cnSerial(0){}
	virtual ~CAsyncMsgFunc(){}
public:

	virtual bool OnEvent(PEVENTHEADER  pEvent)
	{
		PZMQHEADER pMsg = (PZMQHEADER)(pEvent + 1);
		if(m_pObj == nullptr || nullptr == m_pfn) return false;
		
		return (m_pObj->*m_pfn)(pMsg);
	}


	bool Init(TYPE* pObject, PZMQMSGFUNC pfn, int cnThread  = 1, int nGroup  = 0)
	{
		if(nullptr == pObject || nullptr == pfn )
		{
			return false;
		}
		m_pObj = pObject;
		m_pfn = pfn;

		//
		if(nGroup == 0)
			CActiveObject::Init(cnThread);
		else 
		{
			CActiveObject* pObj = nullptr;
			for(int i = 0; i < nGroup; ++i)
			{
				pObj = new CActiveObject;
				if(pObj == nullptr)
					continue;
				if(!pObj->Init(1))
				{
					delete pObj;
					pObj = nullptr;
					continue;
				}
				
				pObj->SetOwner(this);
				m_listActiveObj.push_back(pObj);
				m_cnSerial++;
			}
		}
		return true;	
	}

	bool AddMsg(PZMQHEADER pMsg, int nGroupID = 0)
	{
		auto pBuf = g_pBufferMgr->GetBuffer(sizeof(EVENTHEADER) + sizeof(ZMQHEADER) + pMsg->dwLength);
		if(pBuf == nullptr)
		{
			return false;
		}
		
		pBuf->SetHeaderLen(sizeof(EVENTHEADER));
		
		if(!pBuf->Append(pMsg, sizeof(ZMQHEADER)+ pMsg->dwLength))
		{
			g_pBufferMgr->ReleaseBuffer(pBuf);
			return false;
		}
		
		PEVENTHEADER pEvent = (PEVENTHEADER) pBuf->GetBufPtr();
		pEvent->dwLength = pBuf->GetBufLen() - sizeof(EVENTHEADER);

		CActiveObject* pObj = nullptr;
	
		if(m_cnSerial ==  0)	
			pObj = this;
		else 
		{
			pObj = m_listActiveObj[nGroupID%m_cnSerial];
		}

		if(pObj)
		{
			pObj->PostEvent(pBuf);
		}
	
		g_pBufferMgr->ReleaseBuffer(pBuf);
		return true;
	}
	
private:
	TYPE*							m_pObj;
	PZMQMSGFUNC						m_pfn;
	int								m_cnSerial;
	std::vector<CActiveObject*>		m_listActiveObj;
};



