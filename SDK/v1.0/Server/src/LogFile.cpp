#include "LogFile.h"

#include <time.h>
#include <stdarg.h> 
#include <string.h>
#include <stdio.h> 
#include <fcntl.h> 
#include <thread> 
#include <sys/time.h> 



static char szLogLevel[8][8] = { "ALL", "TRACE", "NOTIC", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" }; 

CLogFile::CLogFile(void)
{
	m_nLogLevelType = LOG_LEVEL_TYPE_INFO;
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
	bool bRet = WriteLogV(nLevelType, pszFmt, args);
	va_end(args);
	return bRet;
}

bool CLogFile::WriteLogV(int nLevelType, const char* pszfmt, va_list args)
{
	if(nLevelType < m_nLogLevelType)
	{
		return false;
	}

	char szData[4000] = { 0 };
	
	//va_start(args, pszfmt);
	vsprintf(szData, pszfmt, args);

	struct timeval tmSec;
	gettimeofday(&tmSec, nullptr);
	time_t tNow = time(NULL);
	struct tm tm_Now; 
	localtime_r(&tNow,  &tm_Now);
	char szTime[64] = { 0 };
	
	sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d.%03ld", tm_Now.tm_year + 1900, tm_Now.tm_mon + 1, tm_Now.tm_mday, tm_Now.tm_hour, tm_Now.tm_min, tm_Now.tm_sec, tmSec.tv_usec/1000);
	//sprintf(szData, "%s [%s] %s\n", szTime, szLogLevel[nLevelType], szData);

	char szThread[16] = { 0 };
	sprintf(szThread, "%08X", (unsigned int)GetThreadID());
	//std::string str;
	//str.append(szTime).append(szThread).append(szLogLevel[nLevelType]).append(szData).append("\n");
	char szLog[4096] = { 0 };	
	sprintf(szLog, "%s %s [%s] %s\n", szTime, szThread, szLogLevel[nLevelType], szData);
//	std::unique_lock<std::mutex> locker(m_clsLock);
//	bool bRet = m_bufLog.AppendFormatText("%s %s [%s] %s\n", szTime, szThread, szLogLevel[nLevelType], szData);
//	m_condLog.notify_one();
	return WriteData(szLog, strlen(szLog));
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
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WriteLogV(LOG_LEVEL_TYPE_FATAL, pszFmt, args);
	va_end(args);
	return bRet;
	//return WriteLog(LOG_LEVEL_TYPE_FATAL, pszFmt, __VA_ARGS__);
}

bool CLogFile::Error(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WriteLogV(LOG_LEVEL_TYPE_ERROR, pszFmt, args);
	va_end(args);
	return bRet;
//	return WriteLog(LOG_LEVEL_TYPE_ERROR, pszFmt, __VA_ARGS__);
}

bool CLogFile::Warn(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WriteLogV(LOG_LEVEL_TYPE_WARN, pszFmt, args);
	va_end(args);
	return bRet;
//	return WriteLog(LOG_LEVEL_TYPE_WARN, pszFmt, __VA_ARGS__);
}

bool CLogFile::Info(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WriteLogV(LOG_LEVEL_TYPE_INFO, pszFmt, args);
	va_end(args);
	return bRet;
	//return WriteLog(LOG_LEVEL_TYPE_INFO, pszFmt, __VA_ARGS__);
}

bool CLogFile::Debug(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WriteLogV(LOG_LEVEL_TYPE_DEBUG, pszFmt, args);
	va_end(args);
	return bRet;
	//return WriteLog(LOG_LEVEL_TYPE_DEBUG, pszFmt, __VA_ARGS__);
}
	
bool CLogFile::Trace(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WriteLogV(LOG_LEVEL_TYPE_TRACE, pszFmt, args);
	va_end(args);
	return bRet;
	///return WriteLog(LOG_LEVEL_TYPE_TRACE, pszFmt, __VA_ARGS__);
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
	return dwLen == dwLength;
}
	
void CLogFile::Run()
{
	if(!OpenFile())
	{
		return ;
	}
	
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
	
void CLogFile::Terminate()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	m_bStop = true;
	m_condLog.notify_all();
}
