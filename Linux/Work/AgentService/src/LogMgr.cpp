#include "LogMgr.h"



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
	return true;
}

bool  CLogMgr::TimeOut(struct tm* pTime)
{
	return true;
}

bool  CLogMgr::GatherLastestLogInfo()
{
	return true;
}


bool CLogMgr::LoadXMLConfigInfo(const char* pszConfigureFileName)
{
	return true;
}
bool CLogMgr::AddLogDir(const char* pszLogDirName)
{
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
