#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <atomic>


#define  ADD_REF(pObj) \
    if(pObj){ pObj->AddRef();}
#define  DEC_REF(pObj) \
    if(pObj){ pObj->Release(); pObj = nullptr;}


class CAvailable 
{
public:
    CAvailable(void);
    ~CAvailable();
public:
    void AddRef();
    void Release();
private:
    std::atomic_int     m_nRef;
};

