/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		BufferMgr.h
 * @brief:		内存池
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-24
 */

#pragma once

#include <vector>
#include <queue>

#include "Buffer.h"
#include "AutoLock.h"

#define BUFFER_GROUP_COUNT		8
#define BUFFER_MGR_CAPABILITY		8*1024*1024

/*
 *Size		Count

 *256		16384 * 2
 *512		8192  * 2
 *1024		4096  * 2
 *2048		2048  * 2
 *4096		1024  * 2
 *8192		512   * 2
 *16384		256   * 2
 *32768		128   * 2
 * */

class CBufferMgr
{
public:
	CBufferMgr();
	virtual	~CBufferMgr();
public:
	CBuffer* GetBuffer(int nLen, const char* pszFileName = "", int nLine = 0);
	
	void	ReleaseBuffer(CBuffer* pBuffer);

	void	ClearAllBuffer();

	void	PrintInfo(CBuffer* pBuf);
private:
	CObjectLock				m_clsLock[BUFFER_GROUP_COUNT];
	std::queue<CBuffer*>	m_listBuf[BUFFER_GROUP_COUNT];
};
extern class CBufferMgr* g_pBufferMgr;
