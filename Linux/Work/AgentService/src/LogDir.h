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
#include <set>
#include <map>
#include <memory>
#include <string.h>
#include "tinyxml2.h"
#include "Buffer.h"



typedef struct stLogDirInfo
{
	char	szDirName[MAX_PATH];
	char	szServiceType[16];
	char	szType[16];
	int		nID;
	int		nTTL;				//过期时间 (秒)
	stLogDirInfo()
	{
		memset(szDirName, 0, sizeof(szDirName));
		memset(szServiceType, 0, sizeof(szServiceType));
		memset(szType, 0, sizeof(szType));
	}
	bool operator < (const stLogDirInfo* pLogDir)
	{
		int nDirRet = strcmp(szDirName, pLogDir->szDirName);
		if(nDirRet == 0)
		{

			int nAppRet = strcmp(szServiceType, pLogDir->szServiceType);
			if(nAppRet == 0)
			{
				int nBusRet = strcmp(szType, pLogDir->szType);
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

typedef struct stLastReadInfo
{
	char	szDirName[MAX_PATH];	
	std::set<std::string>	listIgnore;
	std::vector<LOGINFO>	listReadInfo;
	stLastReadInfo()
	{
		memset(szDirName, 0, sizeof(szDirName));
		listIgnore.clear();
		listReadInfo.clear();
	}
	void Clear()
	{
		memset(szDirName, 0, sizeof(szDirName));
		listIgnore.clear();
		listReadInfo.clear();	
	}
}LASTREADINFO, *PLASTREADINFO;


typedef std::shared_ptr<CLogCollect> LOGPtr;

class CLogDir
{
public:
	CLogDir();
	virtual ~CLogDir();
public:
	bool Init(PLOGDIRINFO pLogDirInfo, std::shared_ptr<LASTREADINFO> pReadDirInfo = nullptr);

	void CheckLogValid();

	void CheckLogList();

	void LoadXMLDirInfo(tinyxml2::XMLElement* pXMLDir);

	void GetDirInfo(std::shared_ptr<LASTREADINFO> pLogDirInfo);

	void GetLastestLogItem();
public:
	void CollectLogItem();	

	//更新文件最新信息
	void UpdateLogInfo(std::map<std::string, LOGPtr>::value_type& value);

	bool SerialMessage(char* pszTopic, CBuffer* pBuf, std::vector<std::string>& listLogItem);

public:
	bool								m_bValid;       //该目录有效性
private:
	int									m_nTTL;			//日志过期时间
	char								m_szService[32];	//日志所属业务类型 
	char								m_szType[32];	//日志所属类型: SYS, BUS
	int									m_nID;			//服务ID 
	char								m_szDirName[MAX_PATH]; //当前日志目录
private:
	std::mutex 							m_clsLock;
	std::map<std::string, LOGPtr>		m_listLogInfo;
	std::set<std::string>				m_listIgnoreLog;
	std::set<std::string>				m_listAllFileName;
};
