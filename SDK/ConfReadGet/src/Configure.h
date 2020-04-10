/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:		Configure.h
 * @brief:		Configure 
 * @author:		nbf,nbf0537@163.com
 * @version:	1.0
 * @date:		2020-04-10
 */


#pragma once 

#include <cstdio> 
#include <cstdlib> 
#include <string> 
#include <mutex> 


#include <vector>
#include <map> 
#include <memory>

enum 
{
	ENUM_INI_CONFIG_FILE_NO_EXIST,			//配置文件不存在 
	ENUM_INI_CONFIG_FILE_OPEN_FAIL,			//配置文件打开失败 
	ENUM_INI_CONFIG_FILE_WRITE_FAIL,		//文件写入失败 
};


typedef struct tagIniEntryInfo
{
	char	szSec[128];
	char	szEntry[128];
	char	szValue[512];
	char	szNote[128];
}INIENTRYINFO, *PINIENTRYINFO;

typedef std::shared_ptr<INIENTRYINFO>	INIENTRYINFOPtr;

class CIniConfigure 
{
public:
	CIniConfigure();
	virtual ~CIniConfigure();
public:
	bool	Init(const char* pszConfigureFileName);
	
	bool	GetIniString(const char* pszSec, const char* pszKey, const char* pszDefault, char* pszValue);

	int		GetIniInt(const char* pszSec, const char* pszKey, const int nDefault = 0);

	double	GetIniDouble(const char* pszSec, const char* pszKey, const double dDefault = 0.0);

	int64_t	GetIniInt64(const char* pszSec, const char* pszKey, const int64_t  llDefault = 0);

	bool	WriteIniString(const char* pszSec, const char* pszKey, const char* pszValue, const char* pszNote = nullptr);

	bool	WriteIniInt(const char* pszSec, const char* pszKey, const int nValue = 0, const char* pszNote = nullptr);

	bool	WirteIniDouble(const char* pszSec, const char* pszKey, const double dValue = 0.0, const char* pszNote = nullptr);
	
	bool	WriteIniInt64(const char* pszSec, const char* pszKey, const int64_t llValue = 0, const char* pszNote = nullptr);

	bool	DelIniSecEntry(const char* pszSec, const char* pszKey);

	bool	DelIniSec(const char* pszSec);
public:
	
	bool	ReadIniFile(const char* pszFileTxt, unsigned int dwFileSize);

	bool	WirteIniFile();
	
private:
	char															m_szIniFileName[256];
	int																m_eErrorType;
	unsigned int													m_tLastModTime;
	std::mutex														m_clsLock;
	std::vector<std::vector<INIENTRYINFOPtr>* >						m_listIniConf;
	std::map<std::string, std::map<std::string, INIENTRYINFOPtr>* >				m_listConfInfo;
};
