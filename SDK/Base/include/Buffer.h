#pragma once

#include <stdio.h>
#include <stdlib.h>
#ifdef _GNU_

#define BOOL	int 
#define TRUE    1
#define FALSE	0
#endif

#define BUFFER_INIT_SIZE 256

class CBuffer
{
public:
	CBuffer(void);
	virtual ~CBuffer(void);
public:
	void	SetExpandLen(int nlenExpand);
	void	SetHeaderLen(int nHeaderLen);
	void	Clear(bool  bFree = false);
public:
	char*	GetBufPtr() { return m_pBuf; }
	char*	GetDataPtr() { if (m_pBuf == NULL) { return NULL; } else { return m_pBuf + m_nlenHeader; } }
	
	bool	Expand(int nExpand);
	bool	ExpandTo(int nLength);
	bool	Exchange(CBuffer* pBuffer);
	
	bool	Append(const void* pBuf, int nlen);
	bool	AppendString(const char* pszBuf);
	bool	AppendFormatText(const char* pszFmt, ...);
	bool	AppendFormatTextV(const char* pszFmt, va_list args);


	int		GetCapability() { return m_nlenCapability; }
	int		GetBufLen() { return m_nlenData; }
private:
	char*		m_pBuf;
	int			m_nlenData;
	int			m_nlenCapability;
	int			m_nlenExpand;
	int			m_nlenHeader;
};
