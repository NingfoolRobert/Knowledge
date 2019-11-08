/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		LogDirMgr.h
 * @brief:		日志目录管理 
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-11-08
 */

#pragma  once 

#include "LogCap.h"
#include <mutex>
#include <memory>
#include <string.h>


typedef struct stLogDirInfo
{
	char	szDirName[MAX_PATH];
	char	szAppType[16];
	char	szBussissType[16];
	int		nID;
	stLogDirInfo()
	{
		memset(szDirName, 0, sizeof(szDirName));
		memset(szAppType, 0, sizeof(szAppType));
		memset(szBussissType, 0, sizeof(szBussissType));
	}
	bool operator < (const stLogDirInfo* pLogDir)
	{
		int nDirRet = strcmp(szDirName, pLogDir->szDirName);
		if(nDirRet == 0)
		{

			int nAppRet = strcmp(szAppType, pLogDir->szAppType);
			if(nAppRet == 0)
			{
				int nBusRet = strcmp(szBussissType, pLogDir->szBussissType);
				if(nBusRet == 0)
				{
					return nID < pLogDir->nID;
				}
				else 
					return nBusRet < 0;
			}
			else 
				return nAppRet < 0;
		}
		else 
			return nDirRet < 0;
	}
}LOGDIRINFO,*PLOGDIRINFO;

typedef std::shared_ptr<CLogCollect> LOGPtr;

class CLogDir
{
public:
	CLogDir();
	virtual ~CLogDir();
public:
	bool Init(PLOGDIRINFO pLogDirInfo, std::set<std::string>& listIgnoreLog);

	bool Init(const char* pszDirName, const char* pszAppType, const char* pszBusiss, int nID, int nTTL = 360);

	bool GetCollectLog(std::vector<LOGPtr>& listLog, std::vector<LOGPtr>& listIgnoreLog);

	void CheckLogValid();

	void GetMsgTopic(char* pszTopic);
public:
	bool AddLogObj(const char* pszLogFileName);

	bool DelLogObj(const char* pszLogFileName);

private:
	int									m_nTTL;			//日志过期时间
	char								m_szApp[32];	//日志所属业务类型 
	char								m_szBusiss[32];	//日志所属类型: SYS, BUS
	int									m_nID;			//服务ID 
private:
	std::mutex 							m_clsLock;
	std::map<std::string, LOGPtr>		m_listLogInfo;
	std::set<std::string>				m_listIgnoreLog;
};
