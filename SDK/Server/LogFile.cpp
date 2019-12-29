#include "LogFile.h"

#include <time.h>
#include <stdarg.h> 
#include <string.h>
#include <stdio.h> 
#include <fcntl.h> 
#include <thread> 



static char szLogLevel[8][8] = { "TRACE", "NOTIC", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" }; 

CLogFile::CLogFile(void)
{

}

CLogFile::~CLogFile(void)
{

}

bool CLogFile::OnInitialUpdate(const char* pszLogFileName)
{
	if(nullptr == pszLogFileName || 0 == strlen(pszLogFileName))
	{
		return false;
	}

	strcpy(m_szLogFileName, pszLogFileName);
	
	if(!CThread::Start())
	{
		return false;
	}
	return true;
}

void CLogFile::SetLevel(int nLevelType/* = LOG_LEVEL_TYPE_INFO*/)
{
	m_nLogLevelType = nLevelType;
}

bool CLogFile::WriteLog(int nLevelType, const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	return WriteLogV(nLevelType, pszFmt, args);
}

bool CLogFile::WriteLogV(int nLevelType, const char* pszfmt, va_list args)
{
	if(nLevelType < m_nLogLevelType)
	{
		return false;
	}

	char szData[1024] = { 0 };

	//va_start(args, pszfmt);
	vsprintf(szData, pszfmt, args);
	va_end(args);

	time_t tNow = time(NULL);
	struct tm tm_Now; 
	localtime_r(&tNow,  &tm_Now);
	sprintf(szData, "%d-%d-%d %d:%d:%d %s %s\n", tm_Now.tm_year + 1900, tm_Now.tm_mon + 1, tm_Now.tm_mday, tm_Now.tm_hour, tm_Now.tm_min, tm_Now.tm_sec, szLogLevel[nLevelType], szData);

	return WriteData(szData, strlen(szData));;

}

bool CLogFile::WriteData(const char* pszData, unsigned int dwLength)
{
	std::unique_lock<std::mutex> locker(m_clsLock);	
	if(!m_bufLog.Append(pszData, dwLength))
	{
		return false;
	}
	m_condLog.notify_one();

	return true;
}

bool CLogFile::Fatal(const char* pszFmt, ...)
{
	return WriteLogV(LOG_LEVEL_TYPE_FATAL, pszFmt, __VA_ARGS__);
}

bool CLogFile::Error(const char* pszFmt, ...)
{
	return WriteLogV(LOG_LEVEL_TYPE_ERROR, pszFmt, __VA_ARGS__);
}

bool CLogFile::Warn(const char* pszFmt, ...)
{
	return WriteLogV(LOG_LEVEL_TYPE_WARN, pszFmt, __VA_ARGS__);
}

bool CLogFile::Info(const char* pszFmt, ...)
{
	return WriteLogV(LOG_LEVEL_TYPE_INFO, pszFmt, __VA_ARGS__);
}

bool CLogFile::Debug(const char* pszFmt, ...)
{
	return WriteLog(LOG_LEVEL_TYPE_DEBUG, pszFmt, __VA_ARGS__);
}
	
bool CLogFile::Trace(const char* pszFmt, ...)
{
	return WriteLog(LOG_LEVEL_TYPE_TRACE, pszFmt, __VA_ARGS__);
}
	
bool CLogFile::OpenFile()
{
	m_hFile = open(m_szLogFileName, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if(m_hFile == -1)
	{
		return false;
	}
	return true;
}

bool CLogFile::WriteLogFile(const char* pszData, unsigned int dwLength)
{
	unsigned int  dwLen = write(m_hFile, pszData,  dwLength);
	return dwLen = dwLength;
}
	
void CLogFile::Run()
{
	CBuffer buf;
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_condLog.wait(locker, [&]()->bool{ return m_bufLog.GetBufLen(); });
		buf.Exchange(&m_bufLog);
		m_bufLog.Clear();
		locker.unlock();
		WriteLogFile(buf.GetBufPtr(), buf.GetBufLen());
		buf.Clear();
	}
}
