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


#include <vector>
#include <string.h>


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
	bool Init(const char* pszLogFileName);

	bool CheckTTL(time_t tCurrent, int nTimeDiff);
	
	bool IsGatherData();

public:
	virtual bool GetLastestLog(std::vector<CBuffer*>& listLogInfo);

private:
	bool		m_bTTL;				//文件是否过期 
	time_t		m_tLastModify;
	long		m_lPosition;
	time_t		m_tLastRead;		//上一次读取时间
	char		m_szLogFileName[MAX_PATH];
};
