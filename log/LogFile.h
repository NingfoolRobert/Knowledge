/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxLogFile.h
 * @brief:xxxLogFile 
 * @author:xxxLogFilenbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-08-01
 */

#pragma once 

#include "SimpleLogFile.h"

enum enumLogLevel{
	LOG_LEVEL_TYPE_NULL,				//未定义 
	LOG_LEVEL_TYPE_TRACE,
	LOG_LEVEL_TYPE_NOTIC,
	LOG_LEVEL_TYPE_DEBUG,		
	LOG_LEVEL_TYPE_INFO, 
	LOG_LEVEL_TYPE_WARN, 
	LOG_LEVEL_TYPE_ERROR, 
	LOG_LEVEL_TYPE_FATAL,
	LOG_LEVEL_TYPE_OFF,
};

class CLogFile 
{
public:
	CLogFile();
	virtual ~CLogFile();
public:
	bool OnInitialUpdate(const char* pszFileAllName);

	void SetLogLineFlags(unsigned int dwLineFlag);

	void SetLogLevel(unsigned int  dwLogLevel) { m_dwLogLevel = dwLogLevel; }

	void Terminate();
public:
	bool WriteLog(const char* pszFmt, ...);
	bool WriteLogV(const char* pszFmt, va_list args);
public:
	bool Trace(const char* pszFmt, ...);
	bool Debug(const char* pszFmt, ...);
	bool Info(const char* pszFmt, ...);
	bool Warn(const char* pszFmt, ...);
	bool Error(const char* pszFmt, ...);
	bool Fatal(const char* pszFmt, ...);
	bool WritelLevelLogV(enumLogLevel nLogLevel, const char* pszFmt, va_list args);
private:
	CSimpleLogFile*				m_pSimpleLogFile;
	unsigned int				m_dwLogLevel;
};

