#include "SimpleLogFile.h"
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <thread>
#include "GFunc.h"
#include <pthread.h>
#include <sys/time.h> 



CSimpleLogFile::CSimpleLogFile():m_fd(-1),m_nRef(1)
{
	m_dwLineFlags = LOGFILE_LINE_FLAG_TYPE_NULL;	
}

CSimpleLogFile::~CSimpleLogFile()
{
	Terminate();
}


bool CSimpleLogFile::InitialUpdate(const char* pszFileAllName)
{
	if(nullptr == pszFileAllName)
		return false;
	strcpy(m_szFileAllName, pszFileAllName);

	char szAllDirPath[256] = { 0 };
	GetDirPath(pszFileAllName, szAllDirPath);
	if(!CreateAllPath(szAllDirPath))
	{
		return false;
	}
	//
	m_fd = open(m_szFileAllName, O_CREAT|O_APPEND|O_WRONLY, 0644);
	if(-1 == m_fd)
	{
		return false;
	}
	
	return true;
}

bool CSimpleLogFile::Write(const char* pszData,  unsigned int dwLength)
{
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

void CSimpleLogFile::Terminate()
{
	close(m_fd);
	m_fd = -1;
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
	int nLen = vsnprintf(szTmp,2047, pszFmt, args) + 1;

	return WriteData(szTmp, nLen);
}
	
bool CSimpleLogFile::GetFileName(char* pszFileAllName)
{
	strcpy(pszFileAllName, m_szFileAllName);
	return true;
}
	
bool CSimpleLogFile::WriteData(const char* pszData, unsigned int dwLength)
{
	if(nullptr == pszData || 0 == dwLength)
		return false;

	//Line Flag
	char szLineTag[128] = { 0 };
	struct timeval tv;
	struct tm tmNow;
	if(m_dwLineFlags & LOGFILE_LINE_FLAG_TYPE_DATE)
	{
		gettimeofday(&tv, nullptr);
		localtime_r(&tv.tv_sec, &tmNow);
		sprintf(szLineTag, "%04d-%02d-%02d ", tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday);
	}
	if(m_dwLineFlags & LOGFILE_LINE_FLAG_TYPE_TIME)
	{
		sprintf(szLineTag + 11, "%02d:%02d:%02d.%03ld ", tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec, tv.tv_usec / 1000);
	}
	if(m_dwLineFlags & LOGFILE_LINE_FLAG_TYPE_THREAD)
	{
		sprintf(szLineTag + 24, "%08x ", (unsigned int)pthread_self());
	}

	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		if(m_dwLineFlags)
			if(!m_pLogBuffer->Append(szLineTag, 33))
				return false;
		//
		if(!m_pLogBuffer->Append(pszData, dwLength))
		{
			return false;
		}
	}

	return g_pLogFileMgr->AddSLF(this);
}
	
int	CSimpleLogFile::AddRef()
{
	return ++m_nRef;
}

void CSimpleLogFile::Release()
{
	if(--m_nRef <= 0)
		delete this;
}



////////////////////////////////////////////////////////////////////////////////
//


class CLogFileMgr*	g_pLogFileMgr = nullptr;
CLogFileMgr::CLogFileMgr():m_bStop(false)
{
	g_pLogFileMgr = this;
}
	
CLogFileMgr::~CLogFileMgr()
{
	
}

bool CLogFileMgr::InitialUpdate()
{
	std::thread tr1(&CLogFileMgr::ActiveWorkLogThread, this);
	tr1.detach();
	return true;
}

	
bool CLogFileMgr::AddSLF(CSimpleLogFile* pLogFile)
{
	if(nullptr == pLogFile)
		return false;
	std::unique_lock<std::mutex> locker(m_clsLock);
	pLogFile->AddRef();
	m_listSLF.push_back(pLogFile);
	m_condLogFile.notify_one();
	return true;
}

void CLogFileMgr::ActiveWorkLogThread()
{
	CSimpleLogFile* pLogFile = nullptr;
	CBuffer buf;
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_condLogFile.wait(locker, [&]()->bool{ return !m_listSLF.empty() || m_bStop; });
		m_clsLock.unlock();
		while(!m_listSLF.empty())
		{
			m_clsLock.lock();
			if(!m_listSLF.empty())
			{
				pLogFile = m_listSLF.front();
				m_listSLF.pop_front();
			}
			m_clsLock.unlock();
			if(nullptr == pLogFile)
				continue;
			pLogFile->m_clsLock.lock();
			buf.Exchange(*(pLogFile->m_pLogBuffer));
			pLogFile->Write(&buf);
			pLogFile->m_clsLock.unlock();
			pLogFile->Release();
			pLogFile = nullptr;
			buf.Clear();
		}
	}
}
	
int	CLogFileMgr::GetSLFCount()
{
	return m_listSLF.size();
}
	
void CLogFileMgr::Stop()
{
	while(0 == m_listSLF.size())
	{
		usleep(10);
	}
	std::unique_lock<std::mutex> locker(m_clsLock);
	m_bStop = true;
	m_condLogFile.notify_all();
}
	
