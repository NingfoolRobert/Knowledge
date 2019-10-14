#include "NotifyMgr.h"
#include "tinyxml2.h"



CNotifyMgr::CNotifyMgr()
{
	m_pContactor = nullptr;
}

CNotifyMgr::~CNotifyMgr()
{
	if(m_pContactor)
	{
		delete m_pContactor;
		m_pContactor = nullptr;
	}

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
	
		XMLElement* pService = doc.FirstChildElement("AlaramService");
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
		
		m_pContactor = new CContactor;
		if(m_pContactor == nullptr)
		{
			LogError("%s(%d) New CContactor fail...", __FILE__, __LINE__);
			return false;
		}

		bool bRet = m_pContactor->Init(szContactorFileName);
		if(!bRet)
			return false;
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
bool CNotifyMgr::Send(const char* pszWarnInfo)
{
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
