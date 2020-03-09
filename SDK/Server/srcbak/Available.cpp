#include "Available.h"

CAvailable::CAvailable(void)
{
    m_nRef = 1;
}
CAvailable::~CAvailable()
{
}

void CAvailable::AddRef()
{
    if(m_nRef <= 0)
    {
        return ;
    }
    m_nRef++;
}

void CAvailable::Release()
{
    m_nRef--;
    if(m_nRef <= 0)
    {
        delete this;
    }
}