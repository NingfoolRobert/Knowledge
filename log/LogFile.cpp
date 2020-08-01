#include "LogFile.h"
#include <vector> 
#include <memory>
#include <string.h> 

static const char* pszLogTag[8] = { "",  "[TRACE]",  "[NOTIC]", "[DEBUG]", "[INFO]", "[WARN]", "[ERROR]", "[FATAL]"};
	
CLogFile::CLogFile()
{
	m_dwLogLevel = LOG_LEVEL_TYPE_NULL;
	m_pSimpleLogFile = nullptr;
}
	
CLogFile::~CLogFile()
{
	if(m_pSimpleLogFile)
		m_pSimpleLogFile->Release();
}

bool CLogFile::OnInitialUpdate(const char* pszFileAllName)
{
	if(nullptr == pszFileAllName)
		return false;
	
	m_pSimpleLogFile = new CSimpleLogFile;
	if(nullptr == m_pSimpleLogFile)
		return false;

	return m_pSimpleLogFile->InitialUpdate(pszFileAllName);
}

void CLogFile::SetLogLineFlags(unsigned int dwLineFlag)
{
	if(m_pSimpleLogFile)
	{
		m_pSimpleLogFile->SetLineFlags(dwLineFlag);
	}
}

void CLogFile::Terminate()
{
	if(m_pSimpleLogFile)
	{
		m_pSimpleLogFile->Release();
		m_pSimpleLogFile = nullptr;
	}

	m_dwLogLevel = LOG_LEVEL_TYPE_OFF;
}

bool CLogFile::WriteLog(const char* pszFmt, ...)
{
	if(nullptr == m_pSimpleLogFile)
		return false;
	//
	va_list args;
	va_start(args, pszFmt);
	bool bRet = m_pSimpleLogFile->WriteLogV(pszFmt, args);
	va_end(args);	
	return bRet;
}

bool CLogFile::WriteLogV(const char* pszFmt, va_list args)
{
	if(nullptr == m_pSimpleLogFile)
		return false;
	return m_pSimpleLogFile->WriteLogV(pszFmt, args);
}

bool CLogFile::WritelLevelLogV(enumLogLevel nLogLevel, const char* pszFmt, va_list args)
{
	if(nLogLevel >m_dwLogLevel)
		return true;
	//
	CBuffer stBuf;
	stBuf.AppendFormatText("%s", pszLogTag[nLogLevel]);
	stBuf.AppendFormatTextV(pszFmt, args);
	stBuf.AppendString("");
	return m_pSimpleLogFile->WriteData(stBuf.GetBufPtr(), stBuf.GetBufLen());
}

bool CLogFile::Trace(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WritelLevelLogV(LOG_LEVEL_TYPE_TRACE, pszFmt, args);
	va_end(args);
	return bRet;
}

bool CLogFile::Debug(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WritelLevelLogV(LOG_LEVEL_TYPE_DEBUG, pszFmt, args);
	va_end(args);
	return bRet;
}

bool CLogFile::Info(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WritelLevelLogV(LOG_LEVEL_TYPE_INFO, pszFmt, args);
	va_end(args);
	return bRet;
}

bool CLogFile::Warn(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WritelLevelLogV(LOG_LEVEL_TYPE_WARN, pszFmt, args);
	va_end(args);
	return bRet;
}

bool CLogFile::Error(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WritelLevelLogV(LOG_LEVEL_TYPE_ERROR, pszFmt, args);
	va_end(args);
	return bRet;
}

bool CLogFile::Fatal(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = WritelLevelLogV(LOG_LEVEL_TYPE_FATAL, pszFmt, args);
	va_end(args);
	return bRet;
}
