/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		AysncMsgFunc.h
 * @brief:		AysncMsgFunc 
 * @author:		AysncMsgFuncnbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2020-04-01
 */ 

#include "ActiveObject.h"
#include "BufferMgr.h"
#include "Protocol.h"
#include <vector> 

template<class TYPE>
class CAsyncMsgFunc:public CActiveObject
{
public:
	typedef bool (TYPE::*PZMQMSGFUNC)(PZMQMSG);
public:

	virtual bool OnEvent(PEVENTHEADER  pEvent)
	{
		PZMQMSG pMsg = (PZMQMSG)(pEvent + 1);
		if(m_pObj == nullptr || nullptr == m_pfn) return false;
		
		return (m_pObj->(*m_pfn))(pMsg);
	}


	bool Init(TYPE* pObject, PZMQMSGFUNC* pfn, int cnThread  = 1, int nGroup  = 0)
	{
		if(nullptr == pObject || nullptr == pfn )
		{
			return false;
		}
		m_pObj = pObject;
		m_pfn = pfn;

		//
		if(nGroup == 0)
			CActiveObject::Init(cnThread, 1, cnThread * 100);
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
		
	}

	bool AddMsg(PZMQMSG pMsg, int nGroupID = 0)
	{
		auto pBuf = g_pBufferMgr->GetBuffer(sizeof(EVENTHEADER) + sizeof(ZMQMSG) + pMsg->dwLength);
		if(pBuf == nullptr)
		{
			return false;
		}
		
		pBuf->SetHeaderLen(sizeof(EVENTHEADER));
		
		if(!pBuf->Append(pMsg, sizeof(ZMQMSG)+ pMsg->dwLength))
		{
			g_pBufferMgr->ReleaseBuffer(pBuf);
			return false;
		}
		
		PEVENTHEADER pEvent = (PEVENTHEADER) pBuf->GetBufPtr();
		pEvent->dwLength = pBuf->GetBufLen() - sizeof(EVENTHEADER);
	
		CActiveObject pObj = nullptr;
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
	}
	
private:
	TYPE*							m_pObj;
	PZMQMSGFUNC						m_pfn;
	int								m_cnSerial;
	std::vector<CActiveObject*>		m_listActiveObj;
};


