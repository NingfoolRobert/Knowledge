/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		Log.h
 * @brief:		日志采集文件
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-11-07
 */
#pragma once 

#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include "Buffer.h"
#include "BufferMgr.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <vector>
#include <string.h>
#include <string>
#include <mutex>
#include "Log.h"


#define MAX_PATH  256

typedef struct stLogInfo 
{
	char		szFileName[MAX_PATH];
	long		lPosition;
	time_t		tLastRead;
	stLogInfo():lPosition(0),tLastRead(0)
	{
		memset(szFileName,0, sizeof(szFileName));
	}
}LOGINFO, *PLOGINFO;


class CLogCollect 
{
public:
	CLogCollect();
	virtual ~CLogCollect();

public:
	bool Init(const char* pszLogFileName,const char* pszLogDir, PLOGINFO  pLogInfo = nullptr);

	bool CheckTTL(time_t tCurrent, int nTimeDiff);
	
	bool IsGatherData();

	bool UpdateFileInfo(time_t tLastModify, long lFileSize, int nTTL = 24 * 60 * 60);			//更新文件信息

	bool GetLogInfo(PLOGINFO  pLogInfo);
public:

	virtual bool GetAugementerLogItem();
	
	virtual bool IsCaptureItem(const char* pszLogItem);

protected:

	bool GetLogItem(std::vector<std::string>& listLogItem,char* pszBuf, int& nResidLen);

private:
	
	int GetMapBlockCount(int& nStartPos, int& nResidLen);
private:
	std::mutex	m_clsLock;
	time_t		m_tLastModify;
	long		m_lPosition;
	time_t		m_tLastRead;		//上一次读取时间
	char		m_szLogFileName[MAX_PATH];
	char		m_szLogDir[MAX_PATH];	//所属目录 
	long		m_lCurrentSize;		//文件当前大小
public:
	bool		m_bCheck;
	bool		m_bTTL;				//文件是否过期 
	static int	m_nPageSize;
};
