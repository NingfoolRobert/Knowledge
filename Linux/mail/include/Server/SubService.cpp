#include "SubService.h"
#include "GlobalVar.h"
#include "Log.h"

CSubService::CSubService()
{

}

CSubService::~CSubService()
{

}

bool CSubService::OnInitualUpdate()
{

	LoadNetSettingsXMLConfig();
	
	return true;
}

bool CSubService::OnTimeOut(struct tm* pTime)
{
	//NetSettings change TODO

	return true;
}

bool CSubService::OnSecondIdle()
{
	return true;
}

bool CSubService::InvokeTerminate()
{
	for(auto it = m_mapSubObj.begin(); it != m_mapSubObj.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_mapSubObj.clear();

	return true;
}

bool CSubService::OnTerminate()
{
	return true;
}

bool CSubService::LoadNetSettingsXMLConfig()
{
	using namespace tinyxml2;

	try
	{
		XMLDocument doc;
		//fstream in(m_szConfigureFileName, ios::binary);
		if(doc.LoadFile(m_szConfigureFileName))
		{
			LogError("[E] Load Configure File(%s) fail.", m_szConfigureFileName);
			return false;
		}
		XMLElement* pRoot = doc.FirstChildElement("Service");
		if(nullptr == pRoot)
		{
			LogWarn("No Find Configure Root.FileName:%s",m_szConfigureFileName);
			return false;
		}
		XMLElement* pNet = pRoot->FirstChildElement("NetSettings");
		if(nullptr == pNet)
		{
			LogWarn("No NetSetting Configure.FileName:%s",m_szConfigureFileName);
			return false;
		}
		for(XMLElement* pSub = pNet->FirstChildElement("Sub"); pSub != nullptr; pSub = pSub->NextSiblingElement())
		{
			if(pSub == nullptr)
			{
				LogWarn("No Subscribe Object. FileName:%s",m_szConfigureFileName);
				break;
			}
			
			//
			CSubObj* pSubObj = new CSubObj;
			if(nullptr == pSubObj)
			{
				LogError("%s(%d) [E] Memory Error. ", __FILE__, __LINE__);
				return false;
			}
			pSubObj->Init(this, pSub);

			m_mapSubObj[pSubObj->m_nType] = pSubObj;			

		}
	}
	catch(std::exception e)
	{
		LogError("[E] Parse XML fail. FileName:%s",m_szConfigureFileName);
	}
	return true;	
}
