#include "Buffer.h"
#include <stdarg.h> 
#include <string.h>
#include <iostream> 
#include <algorithm>
#include <vector> 

CBuffer::CBuffer(void)
{
	m_pBuf = nullptr;
	m_nlenData = 0;
	m_nlenCapability = 0;
	m_nlenHeader = 0;
	m_nlenExpand = BUFFER_INIT_SIZE;
}
	

CBuffer::~CBuffer(void)
{
	if (m_pBuf)
	{
		free(m_pBuf);
		m_pBuf = nullptr;
	}
}

void CBuffer::SetExpandLen(int nlenExpand)
{
	if(0 == nlenExpand)
		return ;
	m_nlenExpand = nlenExpand;
}
	
void CBuffer::AddDataLen(int nlen)
{
	m_nlenData += nlen;
	m_nlenData  = m_nlenData > 0 ? m_nlenData : 0;
}

void CBuffer::Clear(bool bFree /*= false*/)
{
	if (bFree)
	{
		if(m_pBuf)
		{	
			free(m_pBuf);
			m_pBuf = nullptr;
		}
		m_nlenCapability = 0;
	}
	m_nlenHeader = 0;	
	m_nlenData = 0;
}
	
void CBuffer::SetHeaderLen(int nHeaderLen)
{
	m_nlenHeader = nHeaderLen;	
	if(m_pBuf == nullptr && m_nlenHeader > 0)
		Expand(nHeaderLen);
}

bool CBuffer::Expand(int nExpand)
{
	int nTotalLen = nExpand + m_nlenData + m_nlenHeader;
	int nOldSize = m_nlenCapability;
	while (nTotalLen > m_nlenCapability)
	{
		m_nlenCapability += m_nlenExpand;
	}
	//
	if (m_pBuf)
	{
		if (nTotalLen < nOldSize) return true;
		char* ppBuf = (char*)realloc(m_pBuf, m_nlenCapability);
		if (ppBuf == nullptr)
			return false;
		m_pBuf = ppBuf;
	}
	else
	{
		m_pBuf = (char*)malloc(m_nlenCapability);
		if (m_pBuf == nullptr)
			return false;
	}

	return true;
}

bool CBuffer::ExpandTo(int nLength)
{
	int nLen = nLength - m_nlenData - m_nlenHeader;
	if(nLen > 0)
	{
		return Expand(nLen);
	}
	else if (nLen < 0)
	{
		//TODO 更改容量大小 
		return false;
	}
	return false;
}

void CBuffer::Exchange(CBuffer* pBuffer)
{
	if(pBuffer == nullptr || pBuffer == this)
		return ;
	//
	CBuffer  stBuf;
	stBuf.Move(pBuffer);
	
	pBuffer->Move(this);
	
	m_nlenData			= stBuf.GetDataLen();
	m_nlenCapability	= stBuf.GetCapability();
	m_nlenHeader		= stBuf.GetHeaderLen();
	m_nlenExpand		= stBuf.GetExpandLen();
	m_pBuf				= stBuf.Detach();
}

void CBuffer::Move(CBuffer* pBuffer)
{
	if(this == pBuffer || nullptr == pBuffer)
		return ;

	Clear(true);
	
	m_nlenData			= pBuffer->GetDataLen();
	m_nlenHeader		= pBuffer->GetHeaderLen();
	m_nlenCapability	= pBuffer->GetCapability();
	m_nlenExpand		= pBuffer->GetExpandLen();
	m_pBuf				= pBuffer->Detach();
}

bool CBuffer::Append(const void* pBuf, int nlen)
{
	if(pBuf == nullptr)
	{
		return false;
	}

	if(!Expand(nlen))
	{
		return false;
	}

	memmove(m_pBuf + m_nlenHeader + m_nlenData, pBuf, nlen);

	m_nlenData += nlen;
	return true;
}

bool CBuffer::AppendString(const char* pszBuf)
{
	if(nullptr == pszBuf)
	{
		return true;
	}
	//	
	if(m_nlenData + m_nlenHeader > 0 && m_pBuf[m_nlenData + m_nlenHeader - 1] == 0)
	{
		m_nlenData--;
	}
		
	return Append(pszBuf, strlen(pszBuf) + 1);
}
	
bool CBuffer::AppendFormatText(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);
	bool bRet = AppendFormatTextV(pszFmt, args);
	va_end(args);
	return bRet;
}

bool CBuffer::AppendFormatTextV(const char* pszFmt, va_list args)
{
	if(pszFmt == nullptr || 0 == strlen(pszFmt))
	{
		return false;
	}

	va_list copy;
	va_copy(copy, args);
	int nLen =  vsnprintf(NULL, 0, pszFmt, copy);

	if(!Expand(nLen + 1))
	{
		return false;
	}

	int nCount = vsnprintf(m_pBuf + m_nlenHeader + m_nlenData, nLen + 1,  pszFmt, args);
	if(nCount < 0)
		return false;
	m_nlenData += nCount;
	return true;
}
	
char*   CBuffer::Detach()
{
	char* pTmp = m_pBuf;
	m_pBuf = nullptr;
	Clear(true);
	return pTmp;
}
	
void CBuffer::Swap(CBuffer& rhs)
{
	std::swap(m_pBuf, rhs.m_pBuf);
	std::swap(m_nlenCapability, rhs.m_nlenCapability);
	std::swap(m_nlenData, rhs.m_nlenData);
	std::swap(m_nlenExpand, rhs.m_nlenExpand);
	std::swap(m_nlenHeader, rhs.m_nlenHeader);
}
