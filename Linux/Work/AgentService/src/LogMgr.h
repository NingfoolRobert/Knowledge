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

#include "tinyxml2.h"



typedef std::shared_ptr<CLogDir>  LOGDIRPtr;


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

	LOGDIRPtr  FindLogDir(const std::string strDirName);

	bool  GetDirAllFile(std::set<std::string>& listFileName);

	void CollectLastLogItem(const std::map<std::string, LOGDIRPtr>::value_type& value);

	void CheckDirInfo();

	void SaveReadedInfo2File();
protected:
	
	bool LoadXMLConfigInfo(const char* pszConfigureFileName, std::vector<LOGDIRINFO>& listLogDirInfo);
	
	bool UpdateLogDirMgr(std::vector<LOGDIRINFO>& listLogDir);
private:
	time_t						m_tLastModify;
	std::mutex					m_clsLock;
	char						m_szReadedFile[MAX_PATH];
	std::map<std::string, LOGDIRPtr> m_mapLogDir;

	std::map<std::string, std::shared_ptr<LASTREADINFO>> m_listReadedInfo;
};
