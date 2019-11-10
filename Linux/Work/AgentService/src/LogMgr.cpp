#include "LogMgr.h"
#include "CommonHelper.h"



CLogMgr::CLogMgr()
{

}
CLogMgr::~CLogMgr()
{

}

bool  CLogMgr::LoadLastReadLogInfo(const char* pszReadFileInfo)
{
	return true;
}

bool  CLogMgr::Init()
{

	LoadXMLConfigInfo();
	LoadLastReadLogInfo();

	return true;
}

bool  CLogMgr::TimeOut(struct tm* pTime)
{
	if(pTime == nullptr)
		return false;
	
	return true;
}

bool  CLogMgr::GatherLastestLogInfo()
{
	return true;
}


bool CLogMgr::LoadXMLConfigInfo(const char* pszConfigureFileName)
{
	if(nullptr == pszConfigureFileName)
		return false;

	return true;
}

bool CLogMgr::AddLogDir(const char* pszLogDirName)
{
	if(nullptr == pszLogDirName || 0 == strlen(pszLogDirName))
		return false;

	auto it = m_mapLogDir.find(pszLogDirName);
	if(it == m_mapLogDir.end())
	{
		auto pLogDir = std::make_shared<CLogDir>();
		if(pLogDir == nullptr)
		{
			LogError("%s(%d) alloc shared_ptr fail. DirName: %s", __FILE__, __LINE__, pszLogDirName);
			return false;
		}
		// TODO LogDir 初始化 
		
	}
	return true;
}

bool CLogMgr::DelLogDir(const char* pszLogDirName)
{
	return true;
}

bool CLogMgr::AddLogInfo(const char* pszLogFileName, const char* szAppType, const char* pszBusissType)
{
	return true;
}

bool CLogMgr::DelLogInfo(const char* pszLogFileName)
{
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
