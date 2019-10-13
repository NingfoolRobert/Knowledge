#include "Contactor.h"
#include "Log.h"


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


	return true;
}

bool CContactor::GetNotifyObj(std::vector<CONTACTORPtr>& listNotifyObj, int nAlarmLevel, std::string strType)
{
	listNotifyObj.clear();

	if(0 == strType.length())
		return true;
	
	AtomLockRegion locker(&m_clsLock);
	
	auto it = m_mapContactor.find(strType);
	if(it == m_mapContactor.end())
	{
		return false;
	}
	
	for(int i = 0; i < it->second.size(); ++i)
	{
		if(it->second[i]->nLevel <= nAlarmLevel)
		{
			//获取下界
			listNotifyObj.push_back(it->second[i]);				//报警对象级别由低到高
		}
		else 
		{
			break;
		}
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
		
	}
	else 
	{
		m_listContactor[pInfo->nNO] = pUser;
		
	}


	return true;
}

bool CContactor::Del(PCONTACTORINFO pInfo)
{
	if(nullptr == pInfo)
		return false;
	return true;
}

bool CContactor::OnTimeout(struct tm* pTime)
{
	if(nullptr == pTime)
		return false;
	return true;
}

PCONTACTORINFO  CContactor::Find(PCONTACTORINFO pUser)
{
	if(nullptr == pUser)
		return nullptr;
	
	return nullptr;
}

void GetNotifyUser(const char* pszConfigureName)
{
	if(nullptr == pszConfigureName)
	{
		return ;
	}
	
}
