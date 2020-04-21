#include "ActiveObject.h"
#include "BufferMgr.h"
#include "LogFile.h"

#include <sys/select.h> 
#include <sys/time.h> 
#include <thread> 

	
//
CActiveObject::CActiveObject(void):m_bStop(false)
{
	m_pOwner = nullptr;	
}

CActiveObject::~CActiveObject(void)
{
}

	
bool CActiveObject::Init(int cnThread/* = 1*/, CActiveObject* pOwner/* = nullptr*/)
{
	m_cnThread = cnThread;
	m_pOwner = pOwner;
	CAsyncObject::SetOwner(pOwner);	

	for(auto i = 0; i < m_cnThread; ++i)
	{
		std::thread tr1(&CActiveObject::ActiveEventThreadFunc, this);
		tr1.detach();
	}

	SetPendingCnt(1000);
	return true;
}

	
void CActiveObject::SetPendingCnt(int nLen)
{
	m_listEvent.SetExpandLen(nLen);
}
	
void CActiveObject::Terminate()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	CBuffer* pBuffer = m_listEvent.Get();
	while(pBuffer)
	{
		g_pBufferMgr->ReleaseBuffer(pBuffer);
		pBuffer = m_listEvent.Get();
	}
	m_bStop = true;
	m_condEvent.notify_all();
}

bool CActiveObject::PostEvent(PEVENTHEADER pEvent)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	if(nullptr == pEvent)
		return false; 
	
	CBuffer* pBuf = g_pBufferMgr->GetBuffer(pEvent->dwLength + sizeof(EVENTHEADER));
	if(nullptr == pBuf)
	{
		LogError("memory error");
		return false;
	}
	if(!pBuf->Append(pEvent, pEvent->dwLength + sizeof(EVENTHEADER)))
	{
		LogError("memory error");
		g_pBufferMgr->ReleaseBuffer(pBuf);
		return false;
	}
	
	m_listEvent.Add(pBuf);
	m_condEvent.notify_one();
	return true;
}

bool CActiveObject::PostEvent(CBuffer* pBuffer)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	if(nullptr == pBuffer)
		return false; 
	CBuffer* pBuf = g_pBufferMgr->GetBuffer(pBuffer->GetBufLen());
	if(nullptr == pBuf)
	{
		LogError("memory error");
		return false;
	}

	pBuf->Exchange(pBuffer);
	m_listEvent.Add(pBuf);
	m_condEvent.notify_one();
	return true;
}
	
void CActiveObject::ActiveEventThreadFunc()
{
	CBuffer* pBuffer = nullptr;
	PEVENTHEADER pEvent = nullptr;
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_condEvent.wait(locker, [&]()->bool{return !m_listEvent.IsEmpty();});
		locker.unlock();
		while(!m_listEvent.IsEmpty())
		{
			m_clsLock.lock();
			pBuffer = m_listEvent.Get();
			m_clsLock.unlock();
			if(nullptr == pBuffer)
				continue;
			pEvent = (PEVENTHEADER) pBuffer->GetBufPtr();
			OnEvent(pEvent);
			
			g_pBufferMgr->ReleaseBuffer(pBuffer);
		}
	}
}
