#pragma once

#ifndef CLOG_H
#define CLOG_H



#include <fstream>
#include "CICCDateTime.h"
#include "ConcurrentQueueNew.h"
#ifdef WIN32
#include <io.h> 
#include <direct.h>
#else
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h> 
#endif

#define MYLOG CLog::GetInstance()

#pragma pack(1) 
struct LogInfo
{
	unsigned short m_logType;
	char *m_logMessage;
};
#pragma pack() 
namespace CICCTools
{
#define TEST_TYPE 0x01
#define INFO_TYPE 0x02
#define WARN_TYPE 0x04
#define ERROR_TYPE 0x08

	class CLog
	{
	private:
		CLog();
		CLog(CLog&) = delete;
		CLog operator =(CLog) = delete;

	public:
		bool Enable(unsigned short loglevel=255, bool intergration = true);
		bool Disable();		
		bool WriteLog(string log, ...);
		bool Error(string log, ...);
		bool Warn(string log, ...);
		bool Info(string log, ...);
		bool Test(string log, ...);

		static CLog& GetInstance();
	protected:
		bool IntialLogFile(int fileno);		
		bool WriteLogFile(LogInfo*);
		bool WriteLogFileDefault(LogInfo*);
		static void WriteLogThread(void* para);


	private:
		ConcurrentQueueNew<LogInfo*> m_logs;
		char m_day;
		ofstream m_logFile[9];
		bool m_benabled;
		bool m_bintergration;
		int m_currentFileSize;		
		unsigned short m_logLevel;

		static CLog* m_thisLog;

		static const char* mLogType[];

	};
};

#endif
