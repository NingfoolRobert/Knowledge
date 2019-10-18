#include "NotifyMgr.h"
#include "CommonHelper.h"
#include "tinyxml2.h"

#include <string.h>



CNotifyMgr::CNotifyMgr()
{
	m_pContactor = nullptr;
}

CNotifyMgr::~CNotifyMgr()
{
	auto it = m_listNotify.begin(); 
	while(it != m_listNotify.end())
	{
		auto pNotify = *it;
		delete pNotify;
		pNotify = nullptr;
	}

	m_listNotify.clear();
}

bool CNotifyMgr::Init(const char* pszConfigPath)
{
	if(nullptr == pszConfigPath || 0 == strlen(pszConfigPath))
	{
		LogWarn("Configure file is nullptr!");
		return false;
	}
	using namespace tinyxml2;
	XMLDocument doc;
	try 
	{
		if(doc.LoadFile(pszConfigPath))
		{
			LogWarn("Load Configure file(%s) fail...", pszConfigPath);
			return false;
		}

		char szContactorFileName[MAX_PATH] = { 0 };
	
		XMLElement* pService = doc.FirstChildElement("Service");
		if(nullptr == pService)
		{
			LogWarn("The Configure file(%s) no AlarmService Node.", pszConfigPath);
			return false;
		}
		
		XMLElement* pContactor = pService->FirstChildElement("Contactor");
		if(nullptr == pContactor)
		{
			LogWarn("Not find the Node of \" Contactor\" int the Configure file(%s).", pszConfigPath);
			return false;
		}

		strcpy(szContactorFileName, pContactor->Attribute("FileName"));
		
		m_pContactor = std::make_shared<CContactor>();

		if(m_pContactor == nullptr)
		{
			LogError("%s(%d) New CContactor fail...", __FILE__, __LINE__);
			return false;
		}

		bool bRet = m_pContactor->Init(szContactorFileName);
		if(!bRet)
			return false;
		//
		XMLElement* pNotify = pService->FirstChildElement("Notify");
		if(pNotify)
		{
			CNotify* pNotify = CreateNotify(NOTIFY_TYPE_EMAIL);
			if(nullptr != pNotify)
			{
				pNotify->OnInitialUpdate(pszConfigPath);
				m_listNotify.push_back(pNotify);
			}
			else 
			{
				LogError("[E] Create Notify Object fail. Type: %d", NOTIFY_TYPE_EMAIL);
			}
		}
	}
	catch(std::exception e)
	{
		LogError("Read Configure file(%s) occured exception.", pszConfigPath);
	}
	
	return true;
}
	
bool CNotifyMgr::UpdateConfig(const char* pszConfigFileName)
{
	return true;
}

bool CNotifyMgr::Send(const int nLevel, const std::string strAppType, const char* pszWarnInfo)
{
	std::vector<CONTACTORPtr> listUser;
	m_pContactor->GetNotifyList(listUser, nLevel, strAppType);
	
//	for(auto it = m_listNotify.begin(),it != m_listNotify.end(); ++it)
//	{
//		*it->Notify(listUser, pszWarnInfo);
//	}

	for_each(m_listNotify.begin(), m_listNotify.end(), [listUser, pszWarnInfo](CNotify* pNotify){
			pNotify->Notify(listUser,pszWarnInfo);
			});
	return true;
}

bool CNotifyMgr::Add(CNotify* pNotify)
{
	
	



	return true;
}

bool CNotifyMgr::Del(CNotify* pNotify)
{

	return true;
}

CNotify* CNotifyMgr::CreateNotify(int nType)
{
	CNotify* pNotify = nullptr;

	switch(nType)
	{
		case NOTIFY_TYPE_EMAIL:
			pNotify = new CMail;
			break;
		default:
			LogWarn("Create Notify fail. NotifyType: %d", nType);
			break;
	}

	return  pNotify;
}

bool CNotifyMgr::OnTimeout(struct tm* pTime)
{
	//更新告警联系人 
	CheckContator();
	return true;
}

bool CNotifyMgr::CheckContator()
{
	using namespace tinyxml2;
	try
	{
		XMLDocument doc;
		if(doc.LoadFile(m_szConfigureFileName))
		{
			LogError("%s Load Configure file fail.", __FUNCTION__);
			return false;
		}
		
		XMLElement* pRoot  = doc.FirstChildElement("Service");
		if(nullptr == pRoot)
		{
			return false;
		}
		XMLElement* pNotify = pRoot->FirstChildElement("Notify");
		if(nullptr == pNotify)
		{
			return false;
		}
		strcpy(szContatorFileName, pNotify->Attribute("FileName"));
		if(0 == strlen(szContatorFileName))
		{
			return false;
		}

	}
	catch (std::exception e)
	{
		LogWarn("Parse XML File(%s) fail.", m_szConfigureFileName);
		return false;
	}

	char szContatorFileName[MAX_PATH] = { 0 };
	
	time_t tLast;
	time_t tChange;
	time_t tAccess;
	GetFileTime(szContatorFileName, tChange, tLast, tAccess);
	if(tLast == m_tLastContactorFile)
	{
		return true;
	}
	
	auto pContator = std::make_shared<CContactor>();
	if(nullptr == pContator)
	{
		LogError("%s(%d) memory calloc error.", __FILE__, __LINE__);
		return false;
	}

	if(!pContator->Init(szContatorFileName))
	{
		return false;
	}

	m_pContactor = pContator;
	LogInfo("Update Contator Success...");
	return true;	
}
