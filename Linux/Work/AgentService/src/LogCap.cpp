#include "LogCap.h"
#include <fstrema>


CLogCollect::CLogCollect()
{

}

CLogCollect::~CLogCollect()
{

}
	
bool CLogCollect::Init(const char* pszLogFileName)
{
	return true;
}
	
bool CLogCollect::GetLastestLog(std::vector<CBuffer*>& listLogInfo)
{
	if(m_bTTL)
	{
		return false;
	}
	//
	ifstream file(m_szLogFileName);
	if(!file.is_open())
	{
		return false;
	}
	
	//
	file.seekg(0, ios::end);
	long lSize = file.tellg();
	
//
	FILE* fp = fopen(m_szLogFileName, "r+");
	if(nullptr == fp)
	{
		LogError("Open File fail. File:%s", m_szLogFileName);
		return false;
	}
	
		
	
	return true;
}
