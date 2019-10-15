#include "Contactor.h"
#include "Log.h"
#include "tinyxml2.h"
#include "CommonHelper.h"

using namespace CICCTools;
int compContactor(const void* arg1, const void* arg2)
{
//	unsigned int  pUnValue1 = *(unsigned int* )arg1;
//	unsigned int  pUnValue2 = *(unsigned int* )arg2;
//	CONTACTORPtr pUser1 = (CONTACTORPtr)pUnValue1;
	
//	CONTACTORPtr pUser2 = (CONTACTORPtr)pUnValue2;
//	CONTACTORPtr pUser1 = (CONTACTORPtr)arg1;
//	
//	CONTACTORPtr pUser2 = (CONTACTORPtr)arg2;
//
//	if(pUser1->nLevel == pUser2->nLevel)
//	{
//		if(pUser1->nNO < pUser2->nNO)
//			return -1;
//		else if(pUser1->nNO == pUser2->nNO)
//			return 0;
//		else 
//			return 1;
//	}
//	else if(pUser1->nLevel < pUser2->nLevel)
//		return -1;
//	else 
		return 1;
}

CContactor::CContactor()
{

}

CContactor::~CContactor()
{

}



bool CContactor::Init(const char* pszFileName)
{
	if(nullptr == pszFileName || 0 == strlen(pszFileName))
		return false;

	GetNotifyUser(pszFileName);

	return true;
}

bool CContactor::GetNotifyList(std::vector<CONTACTORPtr>& listNotify, int nAlarmLevel, std::string strType)
{
	listNotify.clear();

	if(0 == strType.length())
		return true;
	
	AtomLockRegion locker(&m_clsLock);
	
	auto it = m_mapContactor.find(strType);
	if(it == m_mapContactor.end())
	{
		return false;
	}

	auto pVec = it->second;
	
	for(auto pIt = pVec->begin(); pIt != pVec->end(); ++pIt)
	{
		auto pNotify = *pIt;
		if(pNotify->isNotify(nAlarmLevel))
		{
			listNotify.push_back(*pIt);
		}
		else 
			break;
	}
	
	return true;
}

bool CContactor::Add(PCONTACTORINFO pInfo)
{
	if(nullptr == pInfo)
		return false;

	auto pUser = std::make_shared<CONTACTORINFO>(pInfo);
	if(nullptr == pUser)
	{
		LogInfo("Memory fail!  No:%d,Name:%s,Email:%s",pInfo->nNO, pInfo->szName, pInfo->szEmail);
		return false;
	}
	
	auto it = m_listContactor.find(pInfo->nNO);
	if(it != m_listContactor.end())
	{
		//更新用户信息
		auto pFind = it->second;
		strcpy(pFind->szName, pInfo->szName);
		strcpy(pFind->szEmail, pInfo->szEmail);
		pFind->listFocus.swap(pInfo->listFocus);
	}
	else 
	{
		m_listContactor[pInfo->nNO] = pUser;
		for(int i = 0; pUser->listFocus.size(); ++i)
		{
			std::string strType = pUser->listFocus[i];
			if(strType.length() == 0)
				continue;
			auto itmap = m_mapContactor.find(strType);
			if(itmap == m_mapContactor.end())
			{
				auto pVec = new std::vector<std::shared_ptr<CONTACTORINFO> >;
				if(pVec == nullptr)
				{
					LogWarn("%s(%d) new vector fail.", __FILE__, __LINE__);
					return false;
				}
				pVec->push_back(pUser);
				m_mapContactor[strType] = pVec;
			}
			else 
			{
				auto pVec = itmap->second ;
				pVec->push_back(pUser);
			}
		}	
	}
//	sort(m_listContactor.begin(), m_listContactor.end(), compContactor);
	return true;
}

bool CContactor::Del(PCONTACTORINFO pInfo)
{
	if(nullptr == pInfo)
		return false;
	AtomLockRegion loker(&m_clsLock);
	
	auto it = m_listContactor.find(pInfo->nNO);
	if(it == m_listContactor.end())
	{
		return true;
	}
	else 
	{
		CONTACTORPtr pUser = it->second;
		for(size_t i = 0; i < pUser->listFocus.size(); ++i)
		{
			std::string strType = pUser->listFocus[i];
			auto itType = m_mapContactor.find(strType);
			if(itType == m_mapContactor.end())
				continue;
			auto VecContator = itType->second;
			auto itvec =  VecContator->begin();
			for(; itvec != VecContator->end(); ++itvec)
			{
				auto pNotify = *itvec;
				if(pNotify->nNO == pInfo->nNO)
				{
					VecContator->erase(itvec++);
 					break;
 				} 
			}
		}
		m_listContactor.erase(it);
	}

	
	return true;
}

bool CContactor::OnTimeout(struct tm* pTime)
{
	if(nullptr == pTime)
		return false;
	time_t tModifyTime;
	time_t tChangTime;
	time_t tAccessTime;

	if(CommonHelper::GetFileTime(m_szContactorFileName, tChangTime, tModifyTime, tAccessTime))
	{
		if(tModifyTime != m_tLastModifyTime)
		{
			m_tLastModifyTime = tModifyTime;
			// Update ContactorInfo;
			UpdateContactorInfo();
		}
	}
	
	return true;
}

CONTACTORPtr  CContactor::Find(PCONTACTORINFO pUser)
{
	if(nullptr == pUser)
		return nullptr;
	auto it = m_listContactor.find(pUser->nNO);
	if(it == m_listContactor.end())
		return nullptr;
	
	return it->second;
}

void CContactor::GetNotifyUser(const char* pszConfigureName)
{
	if(nullptr == pszConfigureName || 0 == strlen(pszConfigureName))
	{
		return ;
	}
	using namespace tinyxml2;
	try 
	{
		XMLDocument doc;
		if(doc.LoadFile(pszConfigureName))
		{
			LogWarn("%s(%d) Configure file Load fail. FileName: %s", __FILE__, __LINE__, pszConfigureName);
		}
		
		XMLElement* pContactor = doc.FirstChildElement("Contactor");
		if(nullptr == pContactor)
		{
			return ;
		}
		
		CONTACTORINFO stUser;
		XMLElement* pUser = pContactor->FirstChildElement("User");
		for(; pUser != nullptr; pUser = pUser->NextSiblingElement())
		{
			memset(&stUser, 0, sizeof(stUser));
			
			if(pUser == nullptr)
			{
				break;
			}
			//
			stUser.nNO = atoi( pUser->Attribute("No"));
			strcpy(stUser.szName, pUser->Attribute("Name"));
			strcpy(stUser.szEmail, pUser->Attribute("Email"));
			stUser.nLevel = atoi(pUser->Attribute("Level"));
			//
			XMLElement* pFocus = pUser->FirstChildElement("Focus");
			for(; pFocus != nullptr; pFocus = pFocus->NextSiblingElement())
			{
				if(pFocus == nullptr)
				{
					break;
				}
				//			
				char szTmp[128] = { 0 };
				strcpy(szTmp, pFocus->Attribute("Type"));
				if(strlen(szTmp) != 0)
 					stUser.listFocus.push_back(pFocus->Attribute("Type"));
			}
			Add(&stUser);
		}
	//	sort(m_listContactor.begin(), m_listContactor.end(), );
	}
	catch(std::exception e)
	{
		LogError("Configure Read error. FileName:%s", pszConfigureName);
	}
}

bool  CContactor::UpdateContactorInfo()
{
	AtomLockRegion locker(&m_clsLock);
	GetNotifyUser(m_szContactorFileName);
	return true;
}

