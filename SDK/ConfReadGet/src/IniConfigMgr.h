/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxIniConfigMgr.h
 * @brief:xxxIniConfigMgr
 * @author:xxxIniConfigMgrnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-04-28
 */

#pragma once 

#include "Singleton.h"
#include "Configure.h"


class CIniConfigMgr
{
public:
	CIniConfigMgr();
	virtual ~CIniConfigMgr();
public:
	
	void	OnCheckUpdate();
public:
	bool	GetIniString(const char* pszSection, const char* pszEntry, const char* pszDefault, char* pszValue, const char* pszConfigureFileName);
	int		GetIniInt(const char* pszSection, const char* pszEntry, const int nDefault, const char* pszConfigureFileName);
protected:
	std::shared_ptr<CIniConfigure> GetIniConfigure(const char* pszConfigureFileName);

	unsigned int GetFileModTime(const char* pszConfigureFileName);
private:
	std::map<std::string, std::shared_ptr<CIniConfigure> >	m_mapIniConfigure;
	std::map<std::string,  unsigned int>					m_mapIniModTime;
};

#define GetIniInt(pszSection, pszEntry, nDefault, pszConfigFileName)   Singleton<CIniConfigMgr>::GetInstance().GetIniInt(pszSection, pszEntry, nDefault, pszConfigFileName)

