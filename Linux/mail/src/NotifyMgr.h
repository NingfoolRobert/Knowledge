#pragma once 

#include "mail.h"
#include "CiccAtomLock.h"
#include "Contactor.h"

#include <algorithm>
#include <typeinfo>


typedef std::shared_ptr<CNotify>  CNotifyPtr;


class CNotifyMgr 
{
public:
	CNotifyMgr();
	virtual ~CNotifyMgr();

public:

	bool Init(const char* pszConfigPath);
	
	bool UpdateConfig(const char* pszConfigFileName);

//	bool Send(const int nLevel, const std::string strAppType, const char* pszWarnInfo);
	bool Send(const int nLevel, const std::string strAppType, CBuffer* pBuffer);
	
	bool Add(CNotify* pNotify);

	bool Del(CNotify* pNotify);

	CNotify* CreateNotify(int nType);

	bool OnTimeout(struct tm* pTime);
protected:
	//更新告警联系人
	bool CheckContator();
private:
	
	CICCTools::AtomLock							m_clsLock;
	std::vector<CNotify*>						m_listNotify;	
	std::shared_ptr<CContactor>					m_pContactor;
	time_t										m_tLastContactorFile = 0;
};


