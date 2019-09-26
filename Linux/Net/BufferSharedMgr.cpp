#include "BufferSharedMgr.h"



using namespace std;

CBufferSharedMgr::CBufferSharedMgr()
{

}

CBufferSharedMgr::~CBufferSharedMgr()
{
	for(int i = 0; i < BUFFER_MGR_COUNT; ++i)
	{
		m_listBuffer[i].clear();
	}
}


CBufferShared CBufferSharedMgr::GetBuffer(int nLen, const char* pszFile/* = ""*/, int nLine/* = 0*/)
{
	int cnInit = 256;
	int Index = 0;
	CBufferShared pBuffer = nullptr;
	while(true)
	{
		if(nLen < cnInit)
			break;
		Index++;
		cnInit <<= 1;
	}
	if(Index >= BUFFER_MGR_COUNT)
	{
		pBuffer = std::make_shared<CBufferShared>();
		pBuffer->ExpandTo(nLen);
	}
	else 
	{
		if(m_listBuffer[i].sizeof >= 4*1024*1024 / cnInit)
		{
			auto pBuffer = make_shared<CBuffer>();
			pBuffer->ExpandTo(nLen);
		}
		else 
		{
			pBuffer = m_listBuffer[i].front();
			m_listBuffer[Index].pop_front();
		}
	}
	return pBuffer; 
}
