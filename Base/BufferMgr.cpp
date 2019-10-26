
#include "BufferMgr.h"

CBufferMgr::CBufferMgr()
{
}
CBufferMgr::~CBufferMgr()
{
	CBuffer* pBuf = nullptr;
	for(int i = 0; i < BUFFER_GROUP_COUNT; ++i)
	{
		while(m_listBuf[i].size())
		{
			pBuf = m_listBuf[i].front();
			m_listBuf[i].pop_front();
			delete pBuf;
			pBuf = nullptr;
		}
	}
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
		CAutoLock locker(&m_clsLock[Index]);
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

//*******************************************************
//  @brief: ReleaseBuffer 
 
//  @param pBuffer
// *******************************************************/
void CBufferMgr::ReleaseBuffer(CBuffer* pBuffer)
{
	if(nullptr == pBuffer)	
		return ;
	//
	int nlen = pBuffer->GetCapability();
	if(nlen > BUFFER_MGR_CAPABILITY/BUFFER_INIT_SIZE)	//大于内存池管理大小，直接释放
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

	//当内存池满时，直接释放; 否则，收入内存池中
	CAutoLock locker(&m_clsLock[Index]);
	if(m_listBuf[Index].size() < BUFFER_MGR_CAPABILITY / cnInit)
		m_listBuf[Index].push_back(pBuffer);
	else 
	{
		pBuffer->Clear(true);
		delete pBuffer;
	}
}