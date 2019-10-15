#pragma once 

#include "mail.h"
#include "CiccAtomLock.h"
#include "Contactor.h"

typedef std::shared_ptr<CNotify>  CNotifyPtr;


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

	CNotify* CreateNotify(int nType);

	bool OnTimeout(struct tm* pTime);
private:
	
	CICCTools::AtomLock							m_clsLock;
	std::vector<CNotify*>						m_listNotify;	
	std::shared_ptr<CContactor>					m_pContactor;
};


