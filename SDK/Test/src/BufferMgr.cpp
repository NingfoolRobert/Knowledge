
#include "BufferMgr.h"
//#include "LogFile.h"

class  CBufferMgr* g_pBufferMgr = nullptr;
CBufferMgr::CBufferMgr()
{
	g_pBufferMgr = this;
}

CBufferMgr::~CBufferMgr()
{
	ClearAllBuffer();
}


CBuffer* CBufferMgr::GetBuffer(int nLength, const char* pszFileName, int nLine)
{
	CBuffer* pBuffer = nullptr;
	if(nLength >= BUFFER_MGR_CAPABILITY / BUFFER_INIT_SIZE)
	{
		pBuffer = new CBuffer;
		if(nullptr == pBuffer)
		{
//			LogError("%s(%d) malloc buffer fail.", pszFileName, nLine);
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
	
	CAutoLock locker(&m_clsLock[Index]);
	if(m_listBuf[Index].size() > 0)
	{
		pBuffer =  m_listBuf[Index].front();
		m_listBuf[Index].pop();
		return pBuffer;
	}
	else 
	{
		pBuffer = new CBuffer;
		if(pBuffer == nullptr)
		{
			return pBuffer;
		}
		pBuffer->SetExpandLen(cnInit);
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
		pBuffer->Clear(true);
		delete pBuffer;
		pBuffer = nullptr;
		return ;
	}

	int Index = 0;
	int cnInit = 256;
	while(true)
	{
		if(nlen <= cnInit)
			break;
		Index++;
		cnInit <<= 1;
	}
	if(Index >= BUFFER_GROUP_COUNT)
	{
		pBuffer->Clear(true);
		delete pBuffer;
		pBuffer = nullptr;
		return ;

	}

	//pBuffer->Clear();
	//当内存池满时，直接释放; 否则，收入内存池中
	CAutoLock locker(&m_clsLock[Index]);
	if((int)m_listBuf[Index].size() < BUFFER_MGR_CAPABILITY / cnInit)
	{
		pBuffer->Clear();
		pBuffer->SetExpandLen(BUFFER_INIT_SIZE  << Index);
		m_listBuf[Index].push(pBuffer);
	}
	else 
	{
		pBuffer->Clear(true);
		delete pBuffer;
		pBuffer = nullptr;
	}
}

void CBufferMgr::ClearAllBuffer()
{
	CBuffer* pBuf = nullptr;
	for(int i = 0; i < BUFFER_GROUP_COUNT; ++i)
	{
		CAutoLock locker(&m_clsLock[i]);
		while(m_listBuf[i].size())
		{
			pBuf = m_listBuf[i].front();
			m_listBuf[i].pop();
			delete pBuf;
			pBuf = nullptr;
		}
	}
}
	
void CBufferMgr::PrintInfo(CBuffer* pBuf)
{
	if(nullptr == pBuf)
	{
		return ;
	}

	char szBuf[256] = { 0 };
	
	sprintf(szBuf,"BufferMgr Count: %ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld.", m_listBuf[0].size(), m_listBuf[1].size(), m_listBuf[2].size(), m_listBuf[3].size(), m_listBuf[4].size(), m_listBuf[5].size(), m_listBuf[6].size(), m_listBuf[7].size());
	if(!pBuf->AppendString(szBuf))
	{
//		LogError("Append Data fail.");
	}
}
