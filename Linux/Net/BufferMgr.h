#pragma once

#include <vector>
#include <queue>

#include "Buffer.h"
#include "AutoLock.h"

#define BUFFER_GROUP_COUNT		8

/*
 *Size		Count

 *256		16384
 *512		8192
 *1024		4096
 *2048		2048
 *4096		1024
 *8192		512
 *16384		256
 * */

class CBufferMgr
{
public:
	CBufferMgr();
	virtual	~CBufferMgr();
	CBuffer* GetBuffer(int nLen, const char* pszFileName = "", int nLine = 0);
	void	ReleaseBuffer(CBuffer* pBuffer);
private:
	CObjectLock			  m_clsLock[BUFFER_GROUP_COUNT];
	std::vector<CBuffer*> m_listBuf[BUFFER_GROUP_COUNT];
	

};
