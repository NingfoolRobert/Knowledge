
#include "BufferMgr.h"

CBufferMgr::CBufferMgr()
{
}
CBufferMgr::~CBufferMgr()
{
	
}
CBuffer* CBufferMgr::GetBuffer(int nLength, const char* pszFileName, int nLine)
{
	CBuffer* pBuffer = nullptr;
	if(nLength >= BUFFER_MGR_CAPABILITY / BUFFER_INIT_SIZE)
	{
		pBuffer = new CBuffer;
		if(nullptr == pBuffer)
		{
			//TODO
			return nullptr;
		}
		if(!pBuffer->ExpandTo(nLength))
		{
			return nullptr;
		}
		else 
			return pBuffer;
	}

	int Index = 0;
	int cnInit = 256;
	while(true)
	{
		if(cnInit > nLength)
			break;
		Index++;
		cnInit = cnInit << 1;
	}
	if(m_listBuf[Index].size() > 0)
	{
		pBuffer =  m_listBuf[Index].front();
		m_listBuf[Index].pop_front();
		return pBuffer;
		
	}
	else 
	{
		pBuffer = new CBuffer;
		if(pBuffer == nullptr)
		{
			return pBuffer;
		}
		pBuffer->ExpandTo(nLength);
	}
	return pBuffer;
}
void CBufferMgr::ReleaseBuffer(CBuffer* pBuffer)
{
	if(nullptr == pBuffer)	
		return ;
	//
	int nlen = pBuffer->GetCapability();
	if(nlen > BUFFER_MGR_CAPABILITY/BUFFER_INIT_SIZE)
	{
		delete pBuffer;
		pBuffer = nullptr;
	}
	int Index = 0;
	int cnInit = 256;
	while(true)
	{
		if(nlen >= cnInit)
			break;
		Index++;
		cnInit <<= 1;
	}
	if(Index >= BUFFER_GROUP_COUNT)
	{
		pBuffer->Clear();
		delete pBuffer;
		return ;
	}

	CAutoLock locker(&m_clsLock[Index]);
	if(m_listBuf[Index].size() < BUFFER_MGR_CAPABILITY / cnInit)
		m_listBuf[Index].push_back(pBuffer);
	else 
	{
		pBuffer->Clear(true);
		delete pBuffer;
	}
	pBuffer = nullptr;
}
