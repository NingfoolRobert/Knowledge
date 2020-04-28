#include "IniConfigMgr.h"
#include <string.h> 



	
CIniConfigMgr::CIniConfigMgr()
{

}

CIniConfigMgr::~CIniConfigMgr()
{
	m_mapIniConfigure.clear();
}


	
void CIniConfigMgr::OnCheckUpdate()
{
	for(auto it = m_mapIniModTime.begin(); it != m_mapIniModTime.end(); ++it)
	{
		std::string strFileName = it->first;
		if(it->second == GetFileModTime(strFileName.c_str()))
			continue;
		//
		auto pConf = std::make_shared<CIniConfigure>();
		if(nullptr == pConf)
		{
			continue;
		}
		
		if(!pConf->Init(strFileName.c_str()))
		{
			continue;
		}

		m_mapIniConfigure[strFileName] = pConf;	
	}
}
	
unsigned int CIniConfigMgr::GetFileModTime(const char* pszConfigureFileName)
{
	if(nullptr == pszConfigureFileName || 0 == strlen(pszConfigureFileName))
		return 0;
	//TODO 
	return 0;
}
	
std::shared_ptr<CIniConfigure> CIniConfigMgr::GetIniConfigure(const char* pszConfigureFileName)
{
	if(nullptr == pszConfigureFileName || 0 == strlen(pszConfigureFileName))
		return nullptr;
	auto it = m_mapIniConfigure.find(pszConfigureFileName);
	if(it  == m_mapIniConfigure.end())
		return nullptr;

	return it->second;
}
	
bool CIniConfigMgr::GetIniString(const char* pszSection, const char* pszEntry, const char* pszDefault, char* pszValue, const char* pszConfigureFileName)
{
	if(nullptr == pszConfigureFileName || 0 == strlen(pszConfigureFileName))
		return false;

	auto pConf = GetIniConfigure(pszConfigureFileName);
	if(nullptr != pConf)
	{
		return pConf->GetIniString(pszSection, pszEntry, pszDefault, pszValue);
	}

	auto pConfig = std::make_shared<CIniConfigure>();
	if(nullptr == pConfig)
		return false; 
	
	pConfig->Init(pszConfigureFileName);

	m_mapIniConfigure.insert(std::make_pair(pszConfigureFileName, pConfig));

	return pConfig->GetIniString(pszSection, pszEntry, pszDefault, pszValue);
}

int	CIniConfigMgr::GetIniInt(const char* pszSection, const char* pszEntry, const int nDefault, const char* pszConfigureFileName)
{
	char szTmp[32] = { 0 };
	sprintf(szTmp, "%d",  nDefault);
	char szVal[32] = { 0 };
	if(!GetIniString(pszSection, pszEntry, szTmp, szVal, pszConfigureFileName))
		return nDefault;

	return atoi(szVal);
}
