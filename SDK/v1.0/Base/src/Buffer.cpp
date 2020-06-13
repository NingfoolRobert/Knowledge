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
	
void CBuffer::AddDataLen(int nlen)
{
	m_nlenData += nlen;
	m_nlenData  = m_nlenData > 0 ? m_nlenData : 0;
//capbility 
	int nlenCapability = 0;
	while(nlenCapability <= m_nlenData + m_nlenHeader)
	{
		nlenCapability += m_nlenExpand;
	}

	if(m_pBuf)
		m_nlenCapability = nlenCapability; 
	else 
		m_nlenCapability = 0;
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

bool CBuffer::Exchange(CBuffer* pBuffer)
{
	if(pBuffer == nullptr)
		return false;
	
	
	char* pTmp = m_pBuf;
	int nlenData = m_nlenData;
	int nlenCap = m_nlenCapability;
	int nlenHeader = m_nlenHeader;
	int nExpand = m_nlenExpand;
	
	//
	m_pBuf = pBuffer->GetBufPtr();
	m_nlenData = pBuffer->GetDataLen();
	m_nlenCapability = pBuffer->GetCapability();
	m_nlenHeader = pBuffer->GetHeaderLen();
	m_nlenExpand = pBuffer->GetExpandLen();
	

	char** pObj = pBuffer->Detach();
	*pObj  = nullptr;
	pBuffer->Clear(true);
	*pObj = pTmp;
	pBuffer->SetExpandLen(nExpand);
	pBuffer->SetHeaderLen(nlenHeader);
	pBuffer->AddDataLen(nlenData);
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
	
char**   CBuffer::Detach()
{
	return &m_pBuf;
}
