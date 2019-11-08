/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		LogMgr.cpp
 * @brief:		采集日志管理类
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-11-07
 */

#pragma once 

#include <unistd.h>
#include <set>
#include <mutex>
#include <map>

#include "LogDir.h"



class CLogMgr
{
public:
	CLogMgr();
	~CLogMgr();

public:
	bool  Init();

	bool  TimeOut(struct tm* pTime);

	bool  SecondIdle();			//定时调用，判断是否有新文件产生 
	
	bool  GatherLastestLogInfo();

	//加载上次读取文件信息
	bool  LoadLastReadLogInfo(const char* pszReadFileInfo);
	//保存上次读取文件信息
	bool  SaveLastReadLogInfo();

protected:
	
	bool LoadXMLConfigInfo(const char* pszConfigureFileName);
	
	bool AddLogDir(const char* pszLogDirName);
	
	bool DelLogDir(const char* pszLogDirName);

	bool AddLogInfo(const char* pszLogFileName, const char* szAppType, const char* pszBusissType);

	bool DelLogInfo(const char* pszLogFileName);
	
private:
	std::mutex					m_clsLock;
	std::set<PLOGDIRINFO>		m_listLogDir;
	std::map<std::string , std::set<CLogCollect*>> m_listLogInfo;		//日志以目录管理 

private:
	std::set<PLOGINFO>			m_lisLastReadLogInfo;
	std::set<std::string>		m_listIgnoreLog;
};
