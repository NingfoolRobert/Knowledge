#include <string.h>

#include "Buffer.h"


CBuffer::CBuffer(void)
{
	m_pBuf = nullptr;
	m_nlenData = 0;
	m_nlenCapability = 0;
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
	m_nlenExpand = nlenExpand;
}

void CBuffer::Clear(bool bFree /*= false*/)
{
	if (bFree)
	{
		free(m_pBuf);
		m_pBuf = nullptr;
		m_nlenCapability = 0;
	}

	m_nlenExpand = 256;
	m_nlenData = 0;
}

bool CBuffer::Expand(int nExpand)
{
	int nEnd = nExpand + m_nlenData;
	int nOldSize = m_nlenCapability;
	while (nEnd > m_nlenCapability)
	{
		m_nlenCapability += m_nlenExpand;
	}
	//
	if (m_pBuf)
	{
		if (nEnd < nOldSize) return true;
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

void CBuffer::Exchange(CBuffer& buf)
{
	Clear();
	Append(buf.GetBufPtr(), buf.GetBufLen());
}

bool CBuffer::Append(const void* pBuf, int nlen)
{
	int nTotalLen = m_nlenData + nlen;
	if (m_nlenCapability < nTotalLen)
	{
		while (true)
		{
			if (nTotalLen <= m_nlenCapability)
			{
				break;
			}
			m_nlenCapability += m_nlenExpand;
		}
		//
		if (m_pBuf == nullptr)
		{
			m_pBuf = (char*)malloc(m_nlenCapability);
			if (m_pBuf == nullptr)
				return false;
		}
		else
		{
			char* ppBuf = (char*)realloc(m_pBuf, m_nlenCapability);
			if (ppBuf == nullptr)
				return false;
			m_pBuf = ppBuf;
		}
	}

	memmove(m_pBuf + m_nlenData, pBuf, nlen);

	m_nlenData += nlen;

	return true;
}
