#pragma once 

#include "Notify.h"
#include "CiccAtomLock.h"
class CNotifyMgr 
{
public:
	CNotifyMgr();
	virtual ~CNotifyMgr();

public:
	bool Init(const char* pszConfigPath);
	
	bool Send(const char* pszWarnInfo);
	
	bool Add(CNotify* pNotify);

	bool Del(CNotify* pNotify);

private:

	CICCTools::CiccAtomLock		m_clsLock;
	std::vector<CNotify*>		m_listNotify;	
};


