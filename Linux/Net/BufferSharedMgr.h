#include "Buffer.h"
#include <vector>
#include <iostream> 
#include <mutex>
#include <memory>
#include <chrono>
#include <thread>

#define BUFFER_MGR_COUNT 8


typedef std::shared_ptr<CBuffer> CBufferShared;

class CBufferSharedMgr
{
public:
	CBufferSharedMgr();
	virtual ~CBufferSharedMgr();
public:
	CBufferShared  GetBuffer(int nLen, const char* pszFile = "", int nLine = 0);
	void ReleaseBuffer(CBufferShared pBuf);

private:
	std::mutex					m_clsLock[BUFFER_MGR_COUNT];
	std::vector<CBufferShared>	m_listBuffer[BUFFER_MGR_COUNT];
};
