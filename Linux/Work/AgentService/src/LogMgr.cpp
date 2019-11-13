#include "LogMgr.h"
#include "CommonHelper.h"
#include "tinyxml2.h"
#include <functional>
#include "GlobalVar.h"
#include "AgentService.h"
#include <set>

CLogMgr::CLogMgr()
{
	strcpy(m_szReadedFile, "./ReadFileInfo.xml");
}

CLogMgr::~CLogMgr()
{

}

bool  CLogMgr::LoadLastReadLogInfo(const char* pszReadFileInfo)
{
	if(pszReadFileInfo == nullptr || 0 == strlen(pszReadFileInfo))
	{
		return false;
	}

	m_listReadedInfo.clear();
	using namespace tinyxml2;
	
	try{

		XMLDocument doc;
		if(doc.LoadFile(pszReadFileInfo))
		{
			LogError("%s(%d) File not exist.", __FILE__, __LINE__);
			return false;
		}
		
		XMLElement* pRoot = doc.FirstChildElement("LogFileInfo");
		if(pRoot == nullptr)
		{
			return false;
		}
		
		for(auto pDir = pRoot->FirstChildElement("Dir"); pDir != nullptr; pDir = pDir->NextSiblingElement())
		{
			auto pReadedInfo = std::make_shared<LASTREADINFO>();
			if(pReadedInfo == nullptr)
			{
				LogError("%s(%d) alloc LastReadInfo struct fail.", __FILE__, __LINE__);
				return false;
			}

			auto pDirName = pDir->Attribute("DirName");
			if(pDirName == nullptr)
				continue;
			strcpy(pReadedInfo->szDirName, pDirName);

			auto pRead  = pDir->FirstChildElement("Read");
			if(pRead != nullptr)
			{
				LOGINFO stLogInfo;
				for(auto pLog = pRead->FirstChildElement("Log"); pLog != nullptr; pLog = pLog->NextSiblingElement())
				{
					if(pLog == nullptr)
						break;
					memset(&stLogInfo, 0, sizeof(stLogInfo));
					const char* pFileName = pLog->Attribute("FileName");
					if(pFileName == nullptr)
						continue;
					
					stLogInfo.lPosition = pLog->Int64Attribute("Position", 0);
					stLogInfo.tLastRead = pLog->Int64Attribute("Time", 0);

					pReadedInfo->listReadInfo.push_back(stLogInfo);
				}
			}
			//
			auto pIg = pDir->FirstChildElement("Ignore");
			if(pIg != nullptr)
			{
				for(auto pLog = pIg->FirstChildElement("Log"); pLog != nullptr; pLog = pLog->NextSiblingElement())
				{
					if(pLog == nullptr)
						break;
					
					auto pFileName = pLog->Attribute("FileName");
					if(pFileName != nullptr)
					{
						pReadedInfo->listIgnore.insert(pFileName);
					}

				}
			}
	
			m_listReadedInfo[pReadedInfo->szDirName] = pReadedInfo;
		}
	}
	catch(std::exception e)
	{
		LogError("%s(%d) Parse XML File exception.", __FILE__, __LINE__);
		return false;
	}
	
	return true;
}

bool  CLogMgr::Init()
{
	std::unique_lock<std::mutex> locker(m_clsLock);	
	LoadLastReadLogInfo(m_szReadedFile);

	std::vector<LOGDIRINFO> listLogDir;
	LoadXMLConfigInfo(m_szConfigureFileName, listLogDir);

	UpdateLogDirMgr(listLogDir);

	m_tLastModify = CommonHelper::GetFileModifyTime(m_szConfigureFileName);

	return true;
}

bool  CLogMgr::TimeOut(struct tm* pTime)
{
	if(pTime == nullptr)
		return false;

	//检查文件列表是否更改
	CheckDirInfo();
	//	g_ciccAgentService->PostTask(&CLogMgr::CheckDirInfo, this);
	//保存当前已读取文件信息
	SaveReadedInfo2File();
	//g_ciccAgentService->PostTask(&CLogMgr::SaveReadedInfo2File, this);
	return true;
}

bool  CLogMgr::SecondIdle()			//定时调用，判断是否有新文件产生 
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	
	for(auto it = m_mapLogDir.begin(); it != m_mapLogDir.end(); ++it)
	{
		auto pLog = it->second;	
		pLog->GetLastestLogItem();
	}
	//	for_each(m_mapLogDir.begin(), m_mapLogDir.end(), std::bind(&CLogMgr::CollectLastLogItem,this, std::placeholders::_1));
	return true;	
}


bool  CLogMgr::GatherLastestLogInfo()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	
	m_listReadedInfo.clear();
	
	for(auto it = m_mapLogDir.begin(); it != m_mapLogDir.end(); ++it)
	{
		auto pLastRead = std::make_shared<LASTREADINFO>();
		auto pLogDir = it->second;
		pLogDir->GetDirInfo(pLastRead);
		m_listReadedInfo.insert(make_pair(it->first, pLastRead));
	}

	return true;
}


bool CLogMgr::LoadXMLConfigInfo(const char* pszConfigureFileName, std::vector<LOGDIRINFO>& listLogDirInfo)
{
	if(nullptr == pszConfigureFileName)
		return false;


	std::set<std::string> listDir;
	
	using namespace tinyxml2;
	
	try 
	{
		XMLDocument doc;
		if(doc.LoadFile(pszConfigureFileName))
		{
			LogError("%s(%d) Load XML File fail. FileName: %s", __FILE__, __LINE__, pszConfigureFileName);
			return false;
		}
		
		XMLElement* pService =doc.FirstChildElement("Service");
		if(pService == nullptr)
		{
			LogError("%s(%d) Not Find Root: Service Node.", __FILE__, __LINE__);
			return false;
		}
		
		XMLElement* pDir =pService->FirstChildElement("LogDir");
		if(pDir == nullptr)
		{
			LogError("%s(%d) Not Find Node : LogDir.", __FILE__, __LINE__);
			return false;
		}
	
		LOGDIRINFO  stLogDir;
		for(auto pLogDir = pDir->FirstChildElement("Dir"); pLogDir != nullptr; pLogDir = pLogDir->NextSiblingElement())
		{
			memset(&stLogDir, 0, sizeof(stLogDir));

			if(pLogDir == nullptr)
			{
				break;
			}

			stLogDir.nTTL  = pLogDir->IntAttribute("TTL", 720) * 60;
			
			XMLElement* pDirName = pLogDir->FirstChildElement("DirPath");
			if(pService == nullptr)
			{
				LogError("%s(%d) Not Find Root: Service Node.", __FILE__, __LINE__);
				continue;
			}
			//文件路径
			if(pDirName->GetText() != nullptr)
			{
				if(!CommonHelper::IsDir(pDirName->GetText()))
				{
					LogError("Dir Path is Error. Dir: %s", pDirName->GetText());
					continue;
				}
				if(listDir.count(pDirName->GetText()))
					continue;
				strcpy(stLogDir.szDirName, pDirName->GetText());
				listDir.insert(pDirName->GetText());
			}
			else 
			{
				LogError("%s(%d) LogDirName is NULL.", __FILE__, __LINE__);
				continue;
			}

			//
			XMLElement* pServiceType = pLogDir->FirstChildElement("ServiceType");
			if(pServiceType == nullptr)
			{
				LogError("%s(%d) Not Find Node: ServiceType.", __FILE__, __LINE__);
				continue;
			}
		
			if(pServiceType->GetText() != nullptr)
			strcpy(stLogDir.szServiceType, pServiceType->GetText());

			//
			XMLElement* pType = pLogDir->FirstChildElement("Type");
			if(nullptr == pType)
			{
				LogError("%s(%d) Not Find Node: Type.", __FILE__, __LINE__);
			 	continue;
			}
			
			if(pType->GetText()  != nullptr)
				strcpy(stLogDir.szType, pType->GetText());
			XMLElement* pID = pLogDir->FirstChildElement("ID");
			if(nullptr == pType)
			{
				LogError("%s(%d) Not Find Node: ID.", __FILE__, __LINE__);
			 	continue;
			}	
			char szID[16] = { 0 };
			if(pID->GetText() != nullptr)
			{
				strcpy(szID, pID->GetText());
			}
			stLogDir.nID = atoi(szID);

			listLogDirInfo.push_back(stLogDir);
		}
	}
	catch(std::exception e)
	{
		LogError("%s(%d) Parse XML File fail.", __FILE__, __LINE__);
		return false;
	}
	return true;
}

bool CLogMgr::SaveLastReadLogInfo()
{	
	using namespace tinyxml2;
	XMLDocument doc;	
	const char* declearation = " <?xml version=\"1.0\"encoding=\"UTF-8\"?>";
	doc.Parse(declearation);

	XMLElement* pRoot = doc.NewElement("LogFileInfo");
	
	doc.InsertEndChild(pRoot);
	for(auto it = m_listReadedInfo.begin(); it != m_listReadedInfo.end(); ++it)
	{
		XMLElement* pDir = doc.NewElement("Dir");
		if(pDir == nullptr)
		{
			LogError("%s(%d) Alloc NewElement fail.", __FILE__, __LINE__);
			return false;
		}
		pDir->SetAttribute("DirName",it->first.c_str());


		auto pLastRead = it->second;
		if(pLastRead->listReadInfo.size())
		{
			XMLElement* pRead  = doc.NewElement("Read");
			
			for(auto pReadLog = pLastRead->listReadInfo.begin(); pReadLog != pLastRead->listReadInfo.end(); ++pReadLog)
			{
				
				XMLElement* pLog = doc.NewElement("Log");
				pLog->SetAttribute("FileName", pReadLog->szFileName);
				pLog->SetAttribute("Position", pReadLog->lPosition);
				pLog->SetAttribute("Time", pReadLog->tLastRead);
	
				pRead->InsertEndChild(pLog);
			}
			
			pDir->InsertEndChild(pRead);
		}


		if(pLastRead->listIgnore.size())
		{
			XMLElement* pIgnore = doc.NewElement("Ignore");

			for(auto pIg = pLastRead->listIgnore.begin(); pIg != pLastRead->listIgnore.end(); ++pIg)
			{
				std::string str = *pIg;
				XMLElement* pLog = doc.NewElement("Log");
				pLog->SetAttribute("FileName", str.c_str());
		
				pIgnore->InsertEndChild(pLog);
			}
			pDir->InsertEndChild(pIgnore);
		}
	
		doc.InsertEndChild(pDir);
	}

	//校验已存在文件 	
	if(doc.SaveFile(m_szReadedFile))
	{
		LogError("%s(%d) Save XML File fail.", __FILE__, __LINE__);
		return false;
	}
	
	LogInfo("Monitor Dir Count: %d", m_listReadedInfo.size());
	return true;	
}


LOGDIRPtr  CLogMgr::FindLogDir(const std::string strDirName)
{
	if(0 == strDirName.length())
		return nullptr;
	auto it = m_mapLogDir.find(strDirName);
	
	if(it == m_mapLogDir.end())
		return nullptr;
	return it->second;
}


bool CLogMgr::UpdateLogDirMgr(std::vector<LOGDIRINFO>& listLogDir)
{
	if( 0 == listLogDir.size() )
		return false;
	for_each(m_mapLogDir.begin(), m_mapLogDir.end(), [](const std::map<std::string, LOGDIRPtr>::value_type& value){
			auto pLogDir = value.second;
			pLogDir->m_bValid = false;
			});

	for(size_t i = 0; i < listLogDir.size(); ++i)
	{
		auto stDirInfo = listLogDir[i];

		auto it = m_mapLogDir.find(stDirInfo.szDirName);
		if(it == m_mapLogDir.end())
		{
			auto pLogDir = std::make_shared<CLogDir>();
			if(pLogDir == nullptr)
			{
				LogError("%s(%d) alloc LogDir   Shared pointer fail.", __FILE__, __LINE__);
				continue; 
			}

			auto it = m_listReadedInfo.find(stDirInfo.szDirName);
			if(it == m_listReadedInfo.end())
				
				pLogDir->Init(&stDirInfo, nullptr);
			else 
				pLogDir->Init(&stDirInfo, it->second);

		//	m_mapLogDir[stLogDir.szDirName] = pLogDir;
			m_mapLogDir.insert(make_pair(stDirInfo.szDirName, pLogDir));
		}
		else 
		{
			auto pLogDir = it->second;
			pLogDir->m_bValid = true;
			LogWarn("The LogDir existed.Dir: %s", stDirInfo.szDirName);
		}
	}
	return true;
}


void CLogMgr::CollectLastLogItem(const std::map<std::string, LOGDIRPtr>::value_type& value)
{
	auto pLog = value.second;
	if(nullptr == pLog) 
		return;

	pLog->GetLastestLogItem();
//	g_ciccAgentService->PostTask(&CLogDir::GetLastestLogItem, pLog.get());
}

void CLogMgr::CheckDirInfo()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	
	std::vector<LOGDIRINFO> listDir;
	
	time_t tModify = CommonHelper::GetFileModifyTime(m_szConfigureFileName);
	if(tModify == m_tLastModify)
		return ;
	//
	LoadXMLConfigInfo(m_szConfigureFileName, listDir);
	UpdateLogDirMgr(listDir);
}


void CLogMgr::SaveReadedInfo2File()
{
	GatherLastestLogInfo();
	SaveLastReadLogInfo();
}
