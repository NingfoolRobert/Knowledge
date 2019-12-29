/** 
 * Copyright (C) 2019 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxLogFile.h
 * @brief:xxxLogFile 
 * @author:xxxLogFilenbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2019-12-28
 */
#pragma once 

#include "Thread.h"
#include "Buffer.h"
#include "ObjectLock.h"
#include "Singleton.h"


#include <mutex>
#include <condition_variable> 
#include <unistd.h> 

enum 
{
	LOG_LEVEL_TYPE_TRACE,					//所有日志均打印
	LOG_LEVEL_TYPE_NOTIC,					//最低级别日志
	LOG_LEVEL_TYPE_DEBUG,					//调试日志
	LOG_LEVEL_TYPE_INFO,					//服务运行正常日志
	LOG_LEVEL_TYPE_WARN,					//指定警告
	LOG_LEVEL_TYPE_ERROR,					// 错误时间 允许程序继续运行
	LOG_LEVEL_TYPE_FATAL,					// 非常严重错误， 将导致应用程序终止
	LOG_LEVEL_TYPE_OFF,						//最高等级日志 关闭日志
};

class CLogFile 
{
public:
	CLogFile(void);
	virtual ~CLogFile(void);
public:
	bool OnInitialUpdate(const char* pszLogFileName);
	void SetLevel(int nLevelType = LOG_LEVEL_TYPE_INFO);
	bool WriteLogV(int nLevelType, const char* pszfmt, va_list args);
	bool WriteLog(int nLevelType, const char* pszFmt, ...);
	bool WriteData(const char* pszData, unsigned int dwLength);

	unsigned int GetFileLength() { return m_dwFileLength; }
public:
	bool Fatal(const char* pszFmt, ...);
	bool Error(const char* pszFmt, ...);
	bool Warn(const char* pszFmt, ...);
	bool Info(const char* pszFmt, ...);
	bool Debug(const char* pszFmt, ...);
	bool Trace(const char* pszFmt, ...);
public:
	bool OpenFile();
	bool WriteLogFile(const char* pszData, unsigned int dwLength);
	void Check();
private:
	int					m_nLogLevelType;	
	bool				m_szLogFileName[256];
	CBuffer				m_bufLog;
	unsigned int		m_dwFileLength;
	int					m_hFile;
	std::mutex			m_clsLock;
	std::condition_variable			m_condLog;
	bool				m_bStop;
};



#define LogTrace(fmt, ...) (Singleton<CLogFile>::GetInstance().Trace(fmt, ##__VA_ARGS__)) 
#define LogDebug(fmt, ...) (Singleton<CLogFile>::GetInstance().Debug(fmt, ##__VA_ARGS__)) 
#define LogInfo(fmt, ...) (Singleton<CLogFile>::GetInstance().Info(fmt, ##__VA_ARGS__)) 
#define LogWarn(fmt, ...) (Singleton<CLogFile>::GetInstance().Warn(fmt, ##__VA_ARGS__)) 
#define LogError(fmt, ...) (Singleton<CLogFile>::GetInstance().Error(fmt, ##__VA_ARGS__)) 
#define LogFatal(fmt, ...) (Singleton<CLogFile>::GetInstance().Fatal(fmt, ##__VA_ARGS__)) 



