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
#include <atomic>
#include <condition_variable>
#include <list> 
#include "Buffer.h"

#define  LOGFILE_LINE_FLAG_TYPE_NULL		0  
#define  LOGFILE_LINE_FLAG_TYPE_DATE		0x00000001
#define  LOGFILE_LINE_FLAG_TYPE_TIME		0x00000002 
#define  LOGFILE_LINE_FLAG_TYPE_THREAD		0x00000004 


class CSimpleLogFile 
{
public:
	friend class CLogFileMgr;
public:
	CSimpleLogFile();
	virtual ~CSimpleLogFile();
public:
	bool	InitialUpdate(const char* pszFileAllName);
	void	SetLineFlags(unsigned int dwLineFlag) { m_nLineFlags = dwLineFlag; }
	bool	WriteLog(const char* pszFormat, ...);
	bool	WriteLogV(const char* pszFmt, va_list args);
	bool	WriteData(const char* pszData, unsigned int dwLength);
	void	Terminate();
	bool	GetFileName(char* pszFileAllName);
public:
	int		AddRef();
	void	Release();
protected:
	bool	Write(const char* pszData,  unsigned int dwLength);
	bool	Write(CBuffer* pBuffer);
private:
	int					m_fd;
	int					m_nLineFlags;
	char				m_szFileAllName[256];
	std::mutex			m_clsLock;
	CBuffer*			m_pLogBuffer;
private:
	std::atomic_int		m_nRef;
};


class CLogFileMgr 
{
public:
	CLogFileMgr();
	virtual ~CLogFileMgr();
public:

	bool	Init();

	bool	AddSLF(CSimpleLogFile* pLogFile);

	void	DelSLF(CSimpleLogFile* pLogFile);

	int		GetSLFCount();
	
	void	Stop();
public:
	void	ActiveWorkLogThread();
private:
	bool							m_bStop;
private:
	std::mutex						m_clsLock;
	std::condition_variable			m_condLogFile;
	std::list<CSimpleLogFile* >		m_listSLF;
};
