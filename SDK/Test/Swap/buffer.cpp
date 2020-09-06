#include "buffer.h"
#include <algorithm>


CBuffer::CBuffer()
{

}
	
CBuffer::~CBuffer()
{

}
	
void CBuffer::Set(int nLen, int nCap, int nHeaderLen)
{
	m_nLen = nLen;
	m_nCapbility = nCap;
	m_nHeaderLen = nHeaderLen;
	
}
	
	
void CBuffer::Swap(CBuffer& rhs)
{
	std::swap(m_nHeaderLen, rhs.m_nHeaderLen);
	std::swap(m_nLen, rhs.m_nLen);
	std::swap(m_nCapbility, rhs.m_nCapbility);
}
	
void CBuffer::exchange(CBuffer& rhs) 
{
	int nTmp = rhs.m_nLen;
	rhs.m_nLen = m_nLen;
	m_nLen = nTmp;
	
	nTmp = rhs.m_nCapbility;
	rhs.m_nCapbility = m_nCapbility;
	m_nCapbility = nTmp;
	

	nTmp = rhs.m_nHeaderLen;
	rhs.m_nHeaderLen = m_nHeaderLen;
	m_nHeaderLen = nTmp;
}
