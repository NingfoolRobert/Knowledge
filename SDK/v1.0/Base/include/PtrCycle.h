


#pragma once 
#include <string.h> 


template<class TYPE>
class CPtrCycle 
{
public:
	CPtrCycle(int nExpandLen = 20);
	~CPtrCycle();
public:
	//element size 
	unsigned int	GetSize() const;
	//element count 
	unsigned int	GetCount() const;
	//Cycle empty 
	bool			IsEmpty() const;
	
	//Get and Delete data 
	TYPE*			Get();
	//get head data 
	TYPE*			GetHead();
	
	bool			Add(TYPE* pElement);

	void			SetExpandLen(unsigned int nExpandLen);

	void			RemoveAll(bool bFreeList = false);

	void			DeleteAll(bool bFreeList = false);

	void			FreeAll(bool bFreeList = false);
protected:
	bool			ReallocCapbility();
	
private:
	
	unsigned int	m_nCapLen;
	
	unsigned int	m_nHeadPos;
	
	unsigned int	m_nTailPos;
	
	unsigned int	m_nCount;

	unsigned int	m_nExpandLen;
	
	//element list pointer;
	TYPE**			m_ppElementList;	
};

template<class TYPE>	
CPtrCycle<TYPE>::CPtrCycle(int nExpandLen/* = 20*/)
{
	m_nExpandLen = nExpandLen;
	m_nHeadPos = 0;
	m_nTailPos = 0;
	m_nCount = 0;
	m_nCapLen = 0;
	
}

template<class TYPE>
CPtrCycle<TYPE>::~CPtrCycle()
{
	if(m_ppElementList) 
		free(m_ppElementList);
	
}

//element size 
template<class TYPE>
unsigned int CPtrCycle<TYPE>::GetSize() const 
{
	return m_nCount;
}

//element count 
template<class TYPE>
unsigned int CPtrCycle<TYPE>::GetCount() const 
{
	return m_nCount;
}

//Cycle empty 
template<class TYPE>
bool CPtrCycle<TYPE>::IsEmpty() const 
{
	return m_nCount == 0;
}

//Get and Delete data 
template<class TYPE>
TYPE* CPtrCycle<TYPE>::Get()
{
	if(m_nCount == 0)
		return nullptr;
	TYPE* pElement = m_ppElementList[m_nHeadPos++];
	if(m_nHeadPos >= m_nCapLen)
		m_nHeadPos = 0;
	m_nCount--;
	return pElement;
}

//get head data 
template<class TYPE>
TYPE* CPtrCycle<TYPE>::GetHead()
{
	if(m_nCount == 0)
		return nullptr;
	return m_ppElementList[m_nHeadPos];
}

template<class TYPE>
bool CPtrCycle<TYPE>::Add(TYPE* pElement)
{
	if(!ReallocCapbility())
		return false;
	m_ppElementList[m_nTailPos++] = pElement;
	if(m_nTailPos >= m_nCapLen)
		m_nTailPos = 0;
	m_nCount++;
	return true;
}

template<class TYPE>	
void CPtrCycle<TYPE>::SetExpandLen(unsigned int nExpandLen)
{
	m_nExpandLen = nExpandLen;
}

template<class TYPE>	
bool CPtrCycle<TYPE>::ReallocCapbility()
{
	if(m_nCapLen > m_nCount)
		return true;
	TYPE** ppElementList = (TYPE**)realloc(m_ppElementList, (m_nCapLen + m_nExpandLen) * sizeof(TYPE *));
	if(nullptr == ppElementList)
		return false;
	m_ppElementList = ppElementList;
	if(m_nTailPos < m_nExpandLen)
	{
		memmove(ppElementList + m_nCapLen, ppElementList, m_nTailPos * sizeof(TYPE *));
		m_nTailPos = m_nHeadPos + m_nCount;
	}
	else 
	{
		memmove(ppElementList + m_nCapLen, ppElementList, m_nExpandLen * sizeof(TYPE *));
		memmove(ppElementList, ppElementList + m_nExpandLen, (m_nTailPos - m_nExpandLen) * sizeof(TYPE *));
		m_nTailPos -= m_nExpandLen;
	}
	
	m_nCapLen += m_nExpandLen;
	return true;
}
	
template<class TYPE>
void CPtrCycle<TYPE>::RemoveAll(bool bFreeList/* = false*/)
{
	if(bFreeList)
	{
		if(m_ppElementList) 
		{
			free(m_ppElementList);
			m_ppElementList = nullptr;
			m_nCapLen = 0;
		}
	}
	m_nCount = 0;
	m_nHeadPos = 0;
	m_nTailPos = 0;
}

template<class TYPE>
void CPtrCycle<TYPE>::DeleteAll(bool bFreeList/* = false*/)
{
	TYPE* pElement = Get();
	while(pElement)
	{
		delete pElement;
		pElement = nullptr;
		pElement = Get();
	}
	RemoveAll(bFreeList);
}

template<class TYPE>	
void CPtrCycle<TYPE>::FreeAll(bool bFreeList/* = false*/)
{
	TYPE* pElement = Get();
	while(pElement)
	{
		free(pElement);
		pElement = nullptr;
		pElement = Get();
	}
}
