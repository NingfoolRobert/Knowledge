#include "SimpleLogFile.h"
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
	
CSimpleLogFile::CSimpleLogFile():m_fd(-1)
{
}

CSimpleLogFile::~CSimpleLogFile()
{
	Terminate();
}


bool CSimpleLogFile::Init(const char* pszFileAllName, int nOpenType/* = SIMPLEFILE_APPEND_TYPE_APPEND*/)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	if(nullptr == pszFileAllName)
		return false;
	strcpy(m_szFileAllName, pszFileAllName);
	m_nOpenType = nOpenType;
	if(m_nOpenType == SIMPLEFILE_APPEND_TYPE_OPEN)
	{
		m_fd = open(m_szFileAllName, O_CREAT|O_APPEND|O_WRONLY, 0644);
		if(-1 == m_fd)
		{
			return false;
		}
	}
	
	return true;
}

bool CSimpleLogFile::Write(const char* pszData,  unsigned int dwLength)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	if(nullptr == pszData || 0 == dwLength)
		return false;

	unsigned int dwWrited = write(m_fd, pszData, dwLength);
	
	return dwWrited == dwLength;
}

bool CSimpleLogFile::Write(CBuffer* pBuffer)
{
	if(nullptr == pBuffer)
		return false;
	return Write(pBuffer->GetBufPtr(), pBuffer->GetBufLen());
}

bool CSimpleLogFile::Terminate()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	if(m_nOpenType == SIMPLEFILE_APPEND_TYPE_OPEN)
	{
		close(m_fd);
		m_fd = -1;
	}
	return true;
}

bool CSimpleLogFile::Append(const char* pszData, unsigned int dwLength)
{
	FILE* pFile = fopen(m_szFileAllName, "a+");
	if(nullptr == pFile)
	{
		return false;
	}
	unsigned int dwWrited = fwrite(pszData, 1, dwLength, pFile);

	fclose(pFile);
	return dwWrited == dwLength;
}

bool CSimpleLogFile::Append(CBuffer* pBuffer)
{
	if(pBuffer == nullptr)
		return false;
	
	return Append(pBuffer->GetBufPtr(), pBuffer->GetBufLen());
}
	
bool CSimpleLogFile::WriteLog(const char* pszFormat, ...)
{
	va_list args;
	va_start(args, pszFormat);
	bool bRet = WriteLogV(pszFormat, args);
	va_end(args);
	
	return bRet;
}

bool CSimpleLogFile::WriteLogV(const char* pszFmt, va_list args)
{
	char szTmp[2048] = { 0 };
	vsprintf(szTmp, pszFmt, args);
	if(m_nOpenType == SIMPLEFILE_APPEND_TYPE_OPEN)
	{
		return Write(szTmp, strlen(szTmp));
	}

	return Append(szTmp, strlen(szTmp));
}
	
bool CSimpleLogFile::GetFileName(char* pszFileAllName)
{
	strcpy(pszFileAllName, m_szFileAllName);
	return true;
}
	
bool CSimpleLogFile::SetFileInfo(const char* pszFileAllName, int nAppendType)
{
	//TODO	
	//
	return true;
}
