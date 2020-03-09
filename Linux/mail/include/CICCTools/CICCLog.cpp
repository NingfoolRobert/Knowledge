#include "CICCLog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

using namespace CICCTools;
using namespace std;

CLog* CLog::m_thisLog = NULL;
const char* CLog::mLogType[] = { "", "[TEST]","[INFO]","","[WARN]","","","","[ERROR]" };



CLog::CLog()
{
	m_benabled = false;
}

bool CLog::Enable(unsigned short loglevel, bool intergration)
{
	if (m_benabled)
	{
		return true;
	}	
	m_benabled = true;
	m_bintergration = intergration;
	thread t1(WriteLogThread, this);
	t1.detach();
	m_currentFileSize = 0;
	m_logLevel = loglevel;
	
	return true;
}

bool CICCTools::CLog::Disable()
{
	for (int i = 0; i < 9; i++)
	{
		if (m_logFile[i].is_open())
		{
			m_logFile[i].flush();
			m_logFile[i].close();
		}
	}

	
	m_benabled = false;
	return true;
}

bool CICCTools::CLog::WriteLog(string log, ...)
{

	LogInfo* logstr = (LogInfo*)malloc(sizeof(LogInfo));
	if (logstr == NULL)
	{
		return false;
	}
	int len = log.size();
	len = len * 2 + 1;
	logstr->m_logType = 0;
	logstr->m_logMessage = (char*)malloc(len);
	memset(logstr->m_logMessage, 0, len);
	if (logstr->m_logMessage == NULL)
	{
		return false;
	}

	va_list args;
	while (1)
	{
		va_start(args, log);// ��windows�� args��һ��const char* ����linux ����һ���ṹ��  ��linux����Ҫ����va_start ʹ�ɱ������ָ��ص��ʼ
		int ret = vsnprintf(logstr->m_logMessage, len, log.c_str(), args);
		if (ret < len && ret > 0)
		{
			break;
		}
		else
		{
			len = len * 2;
			logstr->m_logMessage = (char*)realloc(logstr->m_logMessage, len);
			if (logstr->m_logMessage == NULL)
			{
				return false;
			}
			memset(logstr->m_logMessage, 0, len);
		}
	}
	va_end(args);
	return m_logs.Enqueue(logstr);
}

bool CICCTools::CLog::Error(string log, ...)
{
	LogInfo* logstr = (LogInfo*)malloc(sizeof(LogInfo));
	if (logstr == NULL)
	{
		return false;
	}
	int len = log.size();
	len = len * 2 + 1;
	logstr->m_logType = ERROR_TYPE;
	logstr->m_logMessage = (char*)malloc(len);
	memset(logstr->m_logMessage, 0, len);
	if (logstr->m_logMessage == NULL)
	{
		return false;
	}

	va_list args;

	while (1)
	{
		va_start(args, log);// ��windows�� args��һ��const char* ����linux ����һ���ṹ��  ��linux����Ҫ����va_start ʹ�ɱ������ָ��ص��ʼ
		int ret = vsnprintf(logstr->m_logMessage, len, log.c_str(), args);
		if (ret < len && ret > 0)
		{
			break;
		}
		else
		{
			len = len * 2;
			logstr->m_logMessage = (char*)realloc(logstr->m_logMessage, len);
			if (logstr->m_logMessage == NULL)
			{
				return false;
			}
			memset(logstr->m_logMessage, 0, len);
		}
	}
	va_end(args);
	return m_logs.Enqueue(logstr);
}

bool CICCTools::CLog::Warn(string log, ...)
{
	LogInfo* logstr = (LogInfo*)malloc(sizeof(LogInfo));
	if (logstr == NULL)
	{
		return false;
	}
	int len = log.size();
	len = len * 2 + 1;
	logstr->m_logType = WARN_TYPE;
	logstr->m_logMessage = (char*)malloc(len);
	memset(logstr->m_logMessage, 0, len);
	if (logstr->m_logMessage == NULL)
	{
		return false;
	}

	va_list args;
	while (1)
	{
		va_start(args, log);// ��windows�� args��һ��const char* ����linux ����һ���ṹ��  ��linux����Ҫ����va_start ʹ�ɱ������ָ��ص��ʼ
		int ret = vsnprintf(logstr->m_logMessage, len, log.c_str(), args);
		if (ret < len && ret > 0)
		{
			break;
		}
		else
		{
			len = len * 2;
			logstr->m_logMessage = (char*)realloc(logstr->m_logMessage, len);
			if (logstr->m_logMessage == NULL)
			{
				return false;
			}
			memset(logstr->m_logMessage, 0, len);
		}
	}
	va_end(args);
	return m_logs.Enqueue(logstr);
}

bool CICCTools::CLog::Info(string log, ...)
{
	LogInfo* logstr = (LogInfo*)malloc(sizeof(LogInfo));
	if (logstr == NULL)
	{
		return false;
	}
	int len = log.size();
	len = len * 2 + 1;
	logstr->m_logType = INFO_TYPE;
	logstr->m_logMessage = (char*)malloc(len);
	memset(logstr->m_logMessage, 0, len);
	if (logstr->m_logMessage == NULL)
	{
		return false;
	}

	va_list args;
	while (1)
	{
		va_start(args, log);// ��windows�� args��һ��const char* ����linux ����һ���ṹ��  ��linux����Ҫ����va_start ʹ�ɱ������ָ��ص��ʼ
		int ret = vsnprintf(logstr->m_logMessage, len, log.c_str(), args);
		if (ret < len && ret > 0)
		{
			break;
		}
		else
		{
			len = len * 2;
			logstr->m_logMessage = (char*)realloc(logstr->m_logMessage, len);
			if (logstr->m_logMessage == NULL)
			{
				return false;
			}
			memset(logstr->m_logMessage, 0, len);
		}
	}
	va_end(args);
	return m_logs.Enqueue(logstr);
}

bool CICCTools::CLog::Test(string log ...)
{
	LogInfo* logstr = (LogInfo*)malloc(sizeof(LogInfo));
	if (logstr == NULL)
	{
		return false;
	}
	int len = log.size();
	len = len * 2 + 1;
	logstr->m_logType = TEST_TYPE;
	logstr->m_logMessage = (char*)malloc(len);
	memset(logstr->m_logMessage, 0, len);
	if (logstr->m_logMessage == NULL)
	{
		return false;
	}

	va_list args;
	while (1)
	{
		va_start(args, log);// ��windows�� args��һ��const char* ����linux ����һ���ṹ��  ��linux����Ҫ����va_start ʹ�ɱ������ָ��ص��ʼ
		int ret = vsnprintf(logstr->m_logMessage, len, log.c_str(), args);
		if (ret < len && ret > 0)
		{
			break;
		}
		else
		{
			len = len * 2;
			logstr->m_logMessage = (char*)realloc(logstr->m_logMessage, len);// ���ܻ�Ӱ������
			if (logstr->m_logMessage == NULL)
			{
				return false;
			}
			memset(logstr->m_logMessage, 0, len);
		}
	}
	va_end(args);
	return m_logs.Enqueue(logstr);
}


bool CICCTools::CLog::IntialLogFile(int fileno)
{
	string tmpLogFilePath = "./logs";	

#ifdef WIN32
	if (_access(tmpLogFilePath.c_str(), 00) == -1)
	{
		if (_mkdir(tmpLogFilePath.c_str()) == -1)
		{
			return false;
		}
	}

#else
	if (access(tmpLogFilePath.c_str(), F_OK) == -1)
	{
		if (mkdir(tmpLogFilePath.c_str(), 0777) == -1)
		{
			return false;
		}
	}
#endif // WIN32	
	tmpLogFilePath += "/";
	tmpLogFilePath += CICCDateTime::GetCurrentSystemDate("-");
	m_day = CICCDateTime::GetCurrentSystemDate("-").at(9);
	
#ifdef WIN32
	if (_access(tmpLogFilePath.c_str(), 00) == -1)
	{
		if (_mkdir(tmpLogFilePath.c_str()) == -1)
		{
			return false;
		}
	}

#else
	if (access(tmpLogFilePath.c_str(), F_OK) == -1)
	{
		if (mkdir(tmpLogFilePath.c_str(), 0777) == -1)
		{
			return false;
		}
	}
#endif // WIN32

	if (m_logFile[fileno].is_open())
	{
		m_logFile[fileno].close();
	}
	tmpLogFilePath += "/";
	tmpLogFilePath += CICCDateTime::GetCurrentSystemDateTime();
	tmpLogFilePath += mLogType[fileno];
	tmpLogFilePath += ".txt";

	m_logFile[fileno].open(tmpLogFilePath);

	if (!m_logFile[fileno].is_open())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CICCTools::CLog::WriteLogFile(LogInfo * log)
{
	string tmpCurrenrTime = CICCDateTime::GetCurrentSystemDateTimeLogFormat();
	if (tmpCurrenrTime.at(10) != m_day)
	{
		if (!IntialLogFile(log->m_logType))
		{
			return false;
		}
	}
	if (!m_logFile[log->m_logType].is_open())
	{
		if (!IntialLogFile(log->m_logType))
		{
			return false;
		}
	}
	m_logFile[log->m_logType] << tmpCurrenrTime << " "<< mLogType[log->m_logType]<<" " << log->m_logMessage << endl;

	if (m_logFile[log->m_logType].tellp() > 20 * 1024 * 1024)
	{
		return IntialLogFile(log->m_logType);
	}
	return true;
}

bool CICCTools::CLog::WriteLogFileDefault(LogInfo * log)
{
	string tmpCurrenrTime = CICCDateTime::GetCurrentSystemDateTimeLogFormat();
	if (tmpCurrenrTime.at(10) != m_day)
	{
		if (!IntialLogFile(0))
		{
			return false;
		}
	}
	if (!m_logFile[0].is_open())
	{
		if (!IntialLogFile(0))
		{
			return false;
		}
	}
	m_logFile[0] << tmpCurrenrTime << " " << mLogType[log->m_logType] << " " << log->m_logMessage << endl;

	if (m_logFile[0].tellp() > 20 * 1024 * 1024)
	{
		return IntialLogFile(0);
	}

	return true;
}

CLog & CLog::GetInstance()
{
	// TODO: �ڴ˴����� return ���
	if (NULL == m_thisLog)
	{
		m_thisLog = new CLog();
		if (m_thisLog == NULL)
		{
			printf("[ERROR]:CLog:GetInstance: New Fail");
		}
	}
	//printf("MyRedis RedisHelper��ַ%d, CiccRedisClient��ַ%d \n", (int)m_thisHelper, (int)m_thisHelper->m_client);
	return *m_thisLog;
}

void CICCTools::CLog::WriteLogThread(void * para)
{
	try
	{
		CLog *pPushMsg = (CLog *)para;
		const int BUF_SIZE = 1000;
		LogInfo* msgArr[BUF_SIZE];
		if (pPushMsg->m_bintergration)
		{
			while (pPushMsg->m_benabled)
			{
				size_t count = pPushMsg->m_logs.Wait_Dequeue_bulk(msgArr, BUF_SIZE);
				if (count > 0)
				{
					for (int ii = 0; ii < count; ii++)
					{
						LogInfo* msg = msgArr[ii];
						pPushMsg->WriteLogFileDefault(msg);
						free(msg->m_logMessage);
						msg->m_logMessage = nullptr;
						free(msg);
						msg = nullptr;
					}

				}
				else
				{
#ifdef WIN32
					Sleep(100);
#else
					usleep(100 * 1000);
#endif
					
				}
			}
		}
		else
		{
			while (pPushMsg->m_benabled)
			{
				size_t count = pPushMsg->m_logs.Wait_Dequeue_bulk(msgArr, BUF_SIZE);
				if (count > 0)
				{
					for (int ii = 0; ii < count; ii++)
					{
						LogInfo* msg = msgArr[ii];
						pPushMsg->WriteLogFile(msg);
						free(msg);
					}

				}
				else
				{
#ifdef WIN32
					Sleep(100);
#else
					usleep(100 * 1000);
#endif
				}
			}
		}
		

	}
	catch (exception &e)
	{
		printf("Dead Error%s\n", e.what());
	}
	catch (...)
	{
		printf("Dead Unknow Error\n");
	}
}
