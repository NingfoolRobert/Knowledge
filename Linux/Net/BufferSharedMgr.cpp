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
	//CBufferShared pBuffer;
	while(true)
	{
		if(nLen < cnInit)
			break;
		Index++;
		cnInit <<= 1;
	}
	if(Index >= BUFFER_MGR_COUNT)
	{
		auto pBuffer = std::make_shared<CBuffer>();
		pBuffer->ExpandTo(nLen);
		return pBuffer;
	}

	if(m_listBuffer[Index].size() >= 4*1024*1024 / cnInit)
	{
		auto pBuffer = std::make_shared<CBuffer>();
		pBuffer->ExpandTo(nLen);
	}
	else 
	{
		auto pBuffer = m_listBuffer[Index].front();
		m_listBuffer[Index].pop_front();
	}
} 

