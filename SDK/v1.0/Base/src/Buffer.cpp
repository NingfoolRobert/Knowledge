#include "Buffer.h"
#include <stdarg.h> 
#include <string.h>
#include <iostream> 
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
	Expand(nHeaderLen);
	m_nlenData = nHeaderLen;
}

bool CBuffer::Expand(int nExpand)
{
	int nTotalLen = nExpand + m_nlenData;
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
	int nLen = nLength - m_nlenData;
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

bool CBuffer::Exchange(CBuffer* pBuffer)
{
	if(pBuffer == nullptr)
		return false;
	
	Clear();

	if(pBuffer->GetBufPtr() == nullptr)
		return false;
	
	return Append(pBuffer->GetBufPtr(), pBuffer->GetBufLen());
}


bool CBuffer::Append(const void* pBuf, int nlen)
{
	if(pBuf == nullptr)
	{
		return false;
	}
//	int nTotalLen = m_nlenData + nlen;
//	if (m_nlenCapability < nTotalLen)
//	{
//		while (nTotalLen > m_nlenCapability)
//		{
//			m_nlenCapability += m_nlenExpand;
//		}
//		//
//		if (m_pBuf == nullptr)
//		{
//			m_pBuf = (char*)malloc(m_nlenCapability);
//			if (m_pBuf == nullptr)
//				return false;
//		}
//		else
//		{
//			char* ppBuf = (char*)realloc(m_pBuf, m_nlenCapability);
//			if (ppBuf == nullptr)
//				return false;
//			m_pBuf = ppBuf;
//		}
//	}

	if(!Expand(nlen))
	{
		return false;
	}

	memmove(m_pBuf +  m_nlenData, pBuf, nlen);

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
	if(m_nlenData > 0 && m_pBuf[m_nlenData - 1] == 0)
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

	//int nLen = vsnprintf(NULL, 0, pszFmt, args);
	char szData[1024] = { 0 };
	//std::string strData;
	//std::vector<char> strData;
	//strData.resize(nLen + 1);
	vsprintf(szData, pszFmt, args);

	return Append(szData, strlen(szData));
}
