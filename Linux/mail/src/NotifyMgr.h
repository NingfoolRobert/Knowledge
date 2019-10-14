#pragma once 

#include "Notify.h"
#include "CiccAtomLock.h"
#include "Contactor.h"

class CNotifyMgr 
{
public:
	CNotifyMgr();
	virtual ~CNotifyMgr();

public:

	bool Init(const char* pszConfigPath);
	
	bool UpdateConfig(const char* pszConfigFileName);

	bool Send(const char* pszWarnInfo);
	
	bool Add(CNotify* pNotify);

	bool Del(CNotify* pNotify);

private:

	CICCTools::AtomLock			m_clsLock;
	std::vector<CNotify*>		m_listNotify;	
	CContactor*					m_pContactor;
};


