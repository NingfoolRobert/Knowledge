/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxActiveObject.h
 * @brief:xxxActiveObject 
 * @author:xxxActiveObjectnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-04-03
 */
 


#pragma once 

#include "AsyncObject.h"
#include "ObjectLock.h"
#include "PtrCycle.h"
#include "BufferMgr.h"
#include "MemoryReclaimMgr.h"
#include <mutex> 
#include <condition_variable> 
#include <vector> 


class CActiveObject: public CAsyncObject 
{
public:
	CActiveObject();
	virtual ~CActiveObject();
public:
	bool	Init(int cnThread = 1, CActiveObject* pEventOwner = nullptr);
	void	SetOwner(CActiveObject* pOwner);
	void	SetMaxEventType(int nMaxEventType);
	int		GetWaitEventCnt() { return m_listEventCyc.GetCount(); }
	int     GetWaitTimerCnt() { return m_listTimer.size(); }
	void	InvokeStopActiveWork();
	void	WaitStopActiveWork();
public:
	virtual bool PostEvent(PEVENTHEADER pEvent);
	virtual bool PostBuffer(CBuffer* pBuffer);
	
	virtual bool SetTimer(PTIMEHEADER pTimer, int nSecond);
	virtual bool SetTimerMilli(PTIMEHEADER pTimer, int nMilliSec);
	virtual bool KillTimer(unsigned int dwTimerID, int64_t pOwner = 0);
public:
	void	ActiveEventFunc(CActiveObject* pObj);

	void	ActiveTimerFunc(CActiveObject* pObj);
public:

	class CRunable 
	{
	public:
		int		m_nType;
		virtual bool Init(CActiveObject* pObj, const char* pVal, unsigned int dwLength)
		{
			m_pObject = pObj;
			m_pBuffer = g_pBufferMgr->GetBuffer(dwLength);
			if(nullptr == m_pBuffer)
				return false;
			if(!m_pBuffer->Append(pVal, dwLength))
				return false;
			return true;
		}
		virtual bool Run() = 0;
		virtual void Terminate(){g_pBufferMgr->ReleaseBuffer(m_pBuffer); m_pBuffer = nullptr;}	
	protected:
		CActiveObject*	m_pObject;
		CBuffer*		m_pBuffer;
	};

	class CEvent:public CRunable 
	{
		public:
		CEvent(){ m_nType = 0; }
		virtual bool Run() {if(m_pObject && m_pBuffer) return m_pObject->OnEvent((PEVENTHEADER)m_pBuffer->GetBufPtr());}
	};
	
	class CTimer:public  CRunable 
	{
	public:
		CTimer(){m_nType = 1;}
		virtual bool Run() { if(m_pObject && m_pBuffer) return m_pObject->OnTimer((PTIMEHEADER)m_pBuffer->GetBufPtr()); }
		bool IsExpire(int64_t& now) {return now >= m_tExpire;} 
		bool IsSelf(unsigned int dwTimerID, int64_t pOwner) { if(dwTimerID == m_dwTimerID && pOwner == m_pOwner) return true; return false; }
		void SetExpire(int64_t tExpire){ m_tExpire = tExpire; }
	private:
		int64_t				m_tExpire;
		unsigned int		m_dwTimerID;
		int64_t				m_pOwner;
	};
public:	
	
	bool Execute(CRunable* pRunnable);
private: 
	bool								m_bStop;
	std::mutex							m_clsLock;
	std::condition_variable				m_condVar;
	CPtrCycle<CBuffer>					m_listEvent;
	CPtrCycle<CRunable>					m_listEventCyc;
private:
	bool								m_bEnableTimer;
	std::mutex							m_clsTimer;
	std::condition_variable				m_condTimer;
	std::vector<CTimer*>				m_listTimer;
private:
	CMemoryReclaimMgr<CTimer>			m_ContainerTimer;
	CMemoryReclaimMgr<CEvent>			m_ContainerEvent;
private:
	CActiveObject*						m_pEventOwner;
};

