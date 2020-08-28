#include "LogFile.h"

#include <time.h>
#include <stdarg.h> 
#include <string.h>
#include <stdio.h> 
#include <fcntl.h> 
#include <thread> 
#include <sys/time.h> 

static char szDate[16] = { 0 };
static char szTime[16] = { 0 };
static char szLogLevel[8][8] = { "ALL", "TRACE", "NOTIC", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" }; 

static unsigned int  dwDate;
static unsigned int  dwHour;
static unsigned int  dwMin;
static unsigned int  dwSec;

CLogFile::CLogFile(void)
{
	m_nLogLevelType = LOG_LEVEL_TYPE_INFO;
	m_dwLineFlag = LOG_LINEFLAG_TYPE_DATE | LOG_LINGFLAG_TYPE_TIME | LOG_LINGFLAG_TYPE_LEVEL | LOG_LINGFLAG_TYPE_THREADID;
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
	//
	std::unique_lock<std::mutex> locker(m_clsLock);
	struct timeval tmNow;
	gettimeofday(&tmNow, nullptr);

	long tSec = tmNow.tv_sec + 8 * 60 * 60;
	if(m_dwLineFlag & LOG_LINEFLAG_TYPE_DATE)\
	{
		if(dwDate != tSec / (24 * 60 * 60))
		{
			dwDate = tSec / (24 * 60 * 60);
		
			struct tm tTime;
			localtime_r(&tmNow.tv_sec,  &tTime);
			sprintf(szDate, "%04d-%02d-%02d ", tTime.tm_year + 1900, tTime.tm_mon + 1, tTime.tm_mday);
			sprintf(szTime, "%02d:%02d:%02d", tTime.tm_hour, tTime.tm_min, tTime.tm_sec);
		}

		if(!m_bufLog.Append(szDate, 11))
		{
			return false;
		}
	}

	int nDate = tmNow.tv_sec % (24 * 60 * 60);
	if(m_dwLineFlag & LOG_LINGFLAG_TYPE_TIME)
	{
		if(dwHour != nDate / (60 * 60))
		{
			dwHour  = nDate / ( 60 * 60 ) + 8 ;
		}
		int nHour = nDate % (60 * 60);
		if(dwMin != nHour / 60)
		{
			dwMin = nHour / 60;
		}
		if(dwSec != nDate % 60)
		{
			dwSec = nDate % 60;
		}
		//sprintf(szTime, "%02u:%02u:%02u.%03ld ", dwHour, dwMin, dwSec, tmNow.tv_usec / 1000);
		if(!m_bufLog.AppendFormatText("%02d:%02d:%02d.%03ld ", dwHour, dwMin, dwSec, tmNow.tv_usec / 1000))
		{
			return false;
		}
	}

	if(m_dwLineFlag & LOG_LINGFLAG_TYPE_THREADID)
	{
		if(!m_bufLog.AppendFormatText("%08X ", pthread_self()))
		{
			return false;
		}
	}

	if(m_dwLineFlag & LOG_LINGFLAG_TYPE_LEVEL)
	{
		if(!m_bufLog.AppendFormatText("[%s] ", szLogLevel[nLevelType]))
		{
			return false;
		}
	}

	if(!m_bufLog.AppendFormatTextV(pszfmt, args))
	{
		return true;
	}

	m_bufLog.AppendFormatText("\r\n");
	m_condLog.notify_one();
	return true;
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
