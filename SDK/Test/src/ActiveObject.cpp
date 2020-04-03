
#include "ActiveObject.h"
#include <thread> 
#include <sys/time.h> 

CActiveObject::CActiveObject():m_bStop(false),m_bEnableTimer(false)
{

}

CActiveObject::~CActiveObject()
{

}


bool CActiveObject::Init(int cnThread/* = 1*/, CActiveObject* pEventOwner/* = nullptr*/)
{
	for(int i = 0; i < cnThread; ++i)
	{
		std::thread tr1(&CActiveObject::ActiveEventFunc, this, this);
		tr1.detach();
	}

	SetOwner(pEventOwner);
	//
	m_ContainerEvent.SetMallocType(MALLOC_TYPE_NEW);
	m_ContainerTimer.SetMallocType(MALLOC_TYPE_NEW);



	CBufferMgr* pBufferMgr = new CBufferMgr;
	if(nullptr == pBufferMgr)
	{
		return false;
	}
	return true;
}

void CActiveObject::SetOwner(CActiveObject* pOwner)
{
	CAsyncObject::SetOwner(pOwner);
	m_pEventOwner = pOwner;
}

void CActiveObject::SetMaxEventType(int nMaxEventType)
{
	m_listEventCyc.SetExpandLen(nMaxEventType);
}

bool CActiveObject::PostEvent(PEVENTHEADER pEvent)
{
	if(nullptr  == pEvent)
		return false;

	auto pBuffer = g_pBufferMgr->GetBuffer(sizeof(EVENTHEADER) + pEvent->dwLength);
	if(nullptr == pBuffer)
	{
		return false;
	}

	if(!pBuffer->Append(pEvent, sizeof(EVENTHEADER) + pEvent->dwLength))
	{
		return false;
	}

	m_clsLock.lock();
	m_listEvent.Add(pBuffer);
	m_clsLock.unlock();
	m_condVar.notify_one();
//	CEvent* pEve = m_ContainerEvent.GetEmpty();
//	if(nullptr == pEvent)
//	{
//		return false;
//	}
	
//	if(!pEve->Init(this, (char*)pEvent, sizeof(EVENTHEADER) + pEvent->dwLength))
//	{
//		m_ContainerEvent.ReclaimEmpty(pEve);
//		return false;
//	}
//	
//	return Execute(pEve);
	return true;
}

bool CActiveObject::PostBuffer(CBuffer* pBuffer)
{
	if(nullptr == pBuffer)
		return false;
	CEvent* pEvent = m_ContainerEvent.GetEmpty();
	if(nullptr == pEvent)
	{
		return false;
	}
	
	if(!pEvent->Init(this, pBuffer->GetBufPtr(), pBuffer->GetBufLen()))
	{
		m_ContainerEvent.ReclaimEmpty(pEvent);
		return false;
	}
	
	return Execute(pEvent);
}

bool CActiveObject::SetTimer(PTIMEHEADER pTimer, int nSecond)
{
	return SetTimerMilli(pTimer, nSecond * 1000);
}

bool CActiveObject::SetTimerMilli(PTIMEHEADER pTimer, int nMilliSec)
{
	if(nullptr == pTimer) return false;
	std::unique_lock<std::mutex>	locker(m_clsTimer);
	if(!m_bEnableTimer)
	{
		m_bEnableTimer = true;
		std::thread  tr1(&CActiveObject::ActiveTimerFunc, this, this);
		tr1.detach();
	}
	//立即执行
	if(nMilliSec == 0)
	{
		CTimer* pTimer = nullptr;
		//初始化
		return Execute(pTimer);
	}
	//当有该Timer时，修改到期时间 
	struct timeval tnow;
	gettimeofday(&tnow, NULL);
	pTimer->llTimerOver = tnow.tv_sec * 1000 + tnow.tv_usec / 1000 + nMilliSec;
	CTimer*  pTimerEvent = nullptr;
	for(int i = 0; i < m_listTimer.size(); ++i)
	{
		pTimerEvent = m_listTimer[i];
		if(pTimerEvent == nullptr)
		{
			continue;
		}
		
		if(pTimerEvent->IsSelf(pTimer->dwTimerID, pTimer->pOwner))
		{
			struct timeval tnow;
			gettimeofday(&tnow, NULL);
			pTimerEvent->SetExpire(pTimer->llTimerOver);// = tnow.tv_sec * 1000 + tnow.tv_usec / 1000;
			return true;
		}
	}
	// 
	pTimerEvent = m_ContainerTimer.GetEmpty();
	pTimerEvent->Init(this, (char*)pTimer, pTimer->dwLength + sizeof(TIMERHEADER));
	pTimerEvent->SetExpire(pTimer->llTimerOver);
	m_listTimer.push_back(pTimerEvent);
	
	return true;
}

bool CActiveObject::KillTimer(unsigned int dwTimerID, int64_t pOwner /*= 0*/)
{
	if(!m_bEnableTimer)
		return true;

	std::lock_guard<std::mutex>		locker(m_clsTimer);
	CTimer* pTimer = nullptr;
	auto it = m_listTimer.begin(); 
	while(it != m_listTimer.end())
	{
		pTimer = *it;
		if(pTimer == nullptr)
		{
			it++;
			continue;
		}
		//
		if(pTimer->IsSelf(dwTimerID, pOwner))
		{
			m_listTimer.erase(it++);
			pTimer->Terminate();
			m_ContainerTimer.ReclaimEmpty(pTimer);
			return true;
		}
		
	}
	return true;
}
	
void CActiveObject::ActiveEventFunc(CActiveObject* pObj)
{
	CRunable* pRunnable = nullptr;
	CBuffer* pBuffer = nullptr;
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_condVar.wait(locker, [this]()->bool{ return m_listEvent.GetCount();});
		locker.unlock();
		while(m_listEvent.GetCount())
		{
			m_clsLock.lock();
			//pRunnable = m_listEventCyc.Get();
			pBuffer = m_listEvent.Get();
			m_clsLock.unlock();

			if(nullptr == pBuffer)
				continue;

			CAsyncObject::OnEvent((PEVENTHEADER)(pBuffer->GetBufPtr()));


			g_pBufferMgr->ReleaseBuffer(pBuffer);
			pBuffer = nullptr;
			continue;

			if(nullptr == pRunnable)
				break;
			pRunnable->Run();
			pRunnable->Terminate();

//			delete pRunnable;
			pRunnable = nullptr;
			CEvent* pEvent = nullptr;
			CTimer* pTimer = nullptr;
			switch(pRunnable->m_nType)
			{
				case 0:
					pEvent  =dynamic_cast<CEvent*>(pRunnable);
					m_ContainerEvent.ReclaimEmpty(pEvent);
					break;
				case 1:
					pTimer  =dynamic_cast<CTimer*>(pRunnable);
					m_ContainerTimer.ReclaimEmpty(pTimer);
					break;
				default:
					break;
			}
		}
	}
}

void CActiveObject::ActiveTimerFunc(CActiveObject* pObj)
{
	CTimer* pTimer = nullptr;
	struct timeval tnow;
	int64_t tNow = 0;
	while(!m_bEnableTimer)
	{
		//定时基准	
		gettimeofday(&tnow, NULL);
		tNow = tnow.tv_sec * 1000 + tnow.tv_usec / 1000;
		
		std::unique_lock<std::mutex> locker(m_clsTimer);
		for(auto it = m_listTimer.begin(); it != m_listTimer.end(); )
		{
			pTimer = *it;
			if(pTimer == nullptr)
			{
				m_listTimer.erase(it++);
				continue;
			}
			//	
			if(pTimer->IsExpire(tNow))
			{
				m_listTimer.erase(it++);
				Execute(pTimer);
				continue;
			}

			++it;
		}
	}
}
	
bool CActiveObject::Execute(CRunable* pRunnable)
{
	if(nullptr == pRunnable) return false;
	std::unique_lock<std::mutex> locker(m_clsLock);
	
	m_listEventCyc.Add(pRunnable);
	m_condVar.notify_one();
	return true;
}
