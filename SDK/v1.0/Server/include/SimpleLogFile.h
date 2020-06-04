/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		SimpleLogFile.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-05-11
 */


#pragma once 

#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h> 
#include <mutex> 

#include "Buffer.h"


enum 
{
	SIMPLEFILE_APPEND_TYPE_OPEN,			//初始化时打开文件句柄，只到文件关闭 
	SIMPLEFILE_APPEND_TYPE_APPEND,			//默认每次文件写入时，打开文件，随后关闭文件
};


class CSimpleLogFile 
{
public:
	CSimpleLogFile();
	virtual ~CSimpleLogFile();
public:
	bool	Init(const char* pszFileAllName, int nAppendType = SIMPLEFILE_APPEND_TYPE_APPEND);
	bool	SetFileInfo(const char* pszFileAllName, int nAppendType);
	bool	WriteLog(const char* pszFormat, ...);
	bool	WriteLogV(const char* pszFmt, va_list args);
	bool	Terminate();
	bool	GetFileName(char* pszFileAllName);
protected:
	bool	Write(const char* pszData,  unsigned int dwLength);
	bool	Write(CBuffer* pBuffer);
	bool	Append(const char* pszData, unsigned int dwLength);
	bool	Append(CBuffer* pBuffer);
private:
	std::mutex			m_clsLock;
	char				m_szFileAllName[256];
	int					m_fd;
	int					m_nFileAppendType;
};
