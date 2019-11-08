#include "LogDir.h"



CLogDir::CLogDir()
{

}

CLogDir:: ~CLogDir()
{

}

bool CLogDir::Init(PLOGDIRINFO pLogDirInfo, std::set<std::string>& listIgnoreLog)
{
	return true;
}

bool CLogDir::Init(const char* pszDirName, const char* pszAppType, const char* pszBusiss, int nID, int nTTL/* = 360*/)
{
	return true;
}

bool CLogDir::GetCollectLog(std::vector<LOGPtr>& listLog, std::vector<LOGPtr>& listIgnoreLog)
{
	return true;
}

void CLogDir::CheckLogValid()
{

}

void CLogDir::GetMsgTopic(char* pszTopic)
{
	
}

bool CLogDir::AddLogObj(const char* pszLogFileName)
{
	return true;
}

bool CLogDir::DelLogObj(const char* pszLogFileName)
{
	return true;
}





