#include "LogCap.h"
#include <fstream>
#include "Log.h"
#include "AgentService.h"


CLogCollect::CLogCollect()
{

}

CLogCollect::~CLogCollect()
{

}
	
bool CLogCollect::Init(const char* pszLogFileName)
{
	if(nullptr == pszLogFileName)
	{
		return false;
	}
	return true;
}
	
bool CLogCollect::GetLastestLog(std::vector<CBuffer*>& listLogInfo)
{
	if(m_bTTL)
	{
		return false;
	}
	//
	ifstream file(m_szLogFileName);
	if(!file.is_open())
	{
		return false;
	}
	
	//
	file.seekg(0, ios::end);
	long lSize = file.tellg();
	
//
	FILE* fp = fopen(m_szLogFileName, "r+");
	if(nullptr == fp)
	{
		LogError("Open File fail. File:%s", m_szLogFileName);
		return false;
	}
	
		
	
	return true;
}

bool CLogCollect::UpdateFileInfo(time_t tLastModify, long lFileSize)			//更新文件信息
{
	if(lFileSize != m_lCurrentSize)
	{
		m_lCurrentSize = lFileSize;
		m_tLastModify = tLastModify;
		m_bTTL = false;
	}
	else 
	{
		time_t tNow = time(NULL);
		if(tNow - tLastModify > m_nTTL)
			m_bTTL = true;
	}
	return true;
}
bool CLogCollect::GetAugmenterLogItem(std::vector<std::string>& listAugLog)
{
	listAugLog.clear();
	
	int fLog = open(m_szLogFileName, O_RDWR);
	if(fLog <= 0)
	{
		LogInfo("Open file fail. File: %s", m_szLogFileName);
		return false;
	}

	int nReadedSize = 0;

	std::vector<char> Tmp;
	Tmp.resize(m_nPageSize);

	char* pmap = nullptr;
	char* pHead = nullptr;

	int nResidLen = 0;
	int nLength = 0;
	int nStartPos = 0;
	int nMapMinLen = 0;
	int nMapCount = GetMapBlockCount(nStartPos, nMapMinLen);
	int nMapLen = m_nPageSize;
	// mmap	
	for(int i = 0; i < nMapCount; ++i)
	{
		if(i == nMapCount - 1)
		{
			nMapLen =  nMapMinLen;
		}
		
		pmap = (char*)mmap(NULL, nMapLen, PROT_READ | PROT_WRITE, MAP_PRIVATE, fLog, nStartPos + i * m_nPageSize);
		if(pmap == MAP_FAILED)//(void*) -1)
		{
			LogError("%s(%d) memory map fail.", __FILE__, __LINE__);
			continue;
		}
	
		nLength = nResidLen + nMapLen;
		Tmp.resize(nLength + 1);
		memmove(&Tmp[0] + nResidLen, pmap, nMapLen);
		
		munmap(pmap, nMapLen);
		//
		if(i == 0)
		{
			//偏移到上次读取指定位置
			pHead =  &Tmp[0] + (m_lPosition - nStartPos);
		}
		else 
		{
			pHead = &Tmp[0];
		}

		//GetLogItem
		nResidLen = nLength;
		if(!GetLogItem(listAugLog, pHead, nResidLen))
		{
			LogError("%s(%d) Get LogItem fail.", __FILE__, __LINE__);
			break;	
		}

		memset(&Tmp[0], 0, nLength);
		memmove(&Tmp[0], pHead, nResidLen);
		nReadedSize += nLength -  nResidLen;	
	
	
		//Log日志发送
		g_ciccAgentService->SendLogMsg(m_szLogDir, listAugLog);
	}

	m_lPosition  = nStartPos  + nReadedSize; 
	close(fLog);
	return true;
}

bool CLogCollect::IsCaptureItem(const char* pszLogItem)
{
	if(pszLogItem == nullptr)
	{
		return false;
	}

	return true;
}


bool CLogCollect::GetLogItem(std::vector<std::string>& listLogItem, char* pszBuf int& nResidLen)
{
	if(pszBuf == nullptr)
	{
		LogError("%s(%d) Buffer is nullptr.", __FILE__, __LINE__);
		return false;
	}	
	char* pEnd = pszBuf + nLen;
	
	char* pHead = pszBuf;
	char* pTmp =  strchr(pszBuf, '\n');
	if(pTmp == nullptr)
	{
		return true;
	}

	
	while(pTmp)
	{
		*pTmp = 0;
		if(pHead == pEnd)
		{
			return true;
		}
		
		nResidLen -= strlen(pHead) + 1;

		if(IsCaptureItem(pHead))
		{
			listLogItem.emplace_back(pHead);
		}
		pHead = pTmp++;
		pTmp = strchr(pHead, '\n');
	}
	return true;
}

inline int CLogCollect::GetMapBlockCount(int& nStartPos, int& nMapMinLen)
{
	nStartPos = m_lPosition - (m_lPosition / m_nPageSize) * m_nPageSize;
	nMapMinLen = (m_lCurrentSize - nStartPos) % m_nPageSize;
	int nMapCount = (m_lCurrentSize - nStartPos) / m_nPageSize  + nMapMinLen ?  1: 0;
	return nMapCount;
}
