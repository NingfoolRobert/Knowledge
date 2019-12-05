#include "AsyncObject.h"
#include "Log.h"
#include "BufferMgr.h"

#include <thread> 



static void AsyncThread(CAsyncObject* pAsyncObj)
{
	if(pAsyncObj == nullptr)
	{
		return ;
	}
	
	pAsyncObj->AsyncMsgThread();
}

class CAsyncObject* g_ciccAsyncObject = nullptr;

CAsyncObject::CAsyncObject(void):m_pAsyncObj(nullptr)
{
	g_ciccAsyncObject = this;
	m_pAsyncObj = this;
}
CAsyncObject::~CAsyncObject(void)
{
	if(!m_bStop)
		Terminate();
}


bool CAsyncObject::Init(int nThread/* = 1*/, CAsyncObject* pOwner/* = nullptr*/)
{
	for(int i = 0; i < nThread; ++i)
	{
		std::thread tr1(&AsyncThread, this);
		tr1.detach();
	}

	m_pAsyncObj = pOwner;
	return true;
}

bool CAsyncObject::PostMsg(PMSGHEADER pMsg)
{
	if(pMsg == nullptr)
		return false;

	CBuffer* pBuffer = g_pBufferMgr->GetBuffer(pMsg->dwLength + sizeof(MSGHEADER), __FILE__, __LINE__);
	if(nullptr == pBuffer)
	{
		return false;
	}
	if(!pBuffer->Append(pMsg, pMsg->dwLength + sizeof(MSGHEADER)))
	{
		LogError("%s(%d) Append data fail.", __FILE__, __LINE__);
		return false;
	}

	PostMsg(pBuffer);

	return true;
}

bool CAsyncObject::PostMsg(CBuffer* pBuffer)
{
	if(pBuffer == nullptr)
	{
		return false;
	}

	std::unique_lock<std::mutex> locker(m_clsLock);
	m_listMessage.push(pBuffer);
	m_condLock.notify_one();
	return true;
}

bool CAsyncObject::OnMsg(PMSGHEADER pMsg)
{
	if(m_pAsyncObj  == nullptr)
	{
		return false;
	}

	return m_pAsyncObj->OnMsg(pMsg);
}

void CAsyncObject::Terminate()
{
	if(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_bStop = true;
		m_condLock.notify_all();
	}
}

bool CAsyncObject::AsyncMsgThread()
{
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_condLock.wait(locker, [&]()->bool{ return !m_listMessage.empty(); });
		locker.unlock();
		while(!m_listMessage.empty())
		{
			m_clsLock.lock();
			auto pBuf = m_listMessage.front();
			m_listMessage.pop();
			m_clsLock.unlock();
			PMSGHEADER pMsg = (PMSGHEADER)pBuf->GetBufPtr();
			if(pMsg == nullptr)
				continue;
			auto bRet = OnMsg(pMsg);
			if(!bRet)
			{
				;//
			}
			//回收消息内存结构 
			g_pBufferMgr->ReleaseBuffer(pBuf);
		}
	}
	return true;
}

bool CAsyncObject::SetOwner(CAsyncObject* pOwner)
{
	if(pOwner == nullptr)
	{
		return false;
	}

	m_pAsyncObj = pOwner;

	return true;
}
