#include "GFunc.h"
#include <string.h>
#include <unistd.h>
#include "Log.h"

bool GlobalBegin(const char* pszSimpleName)
{
	if(pszSimpleName == nullptr || 0 == strlen(pszSimpleName))
		return false;
	sprintf(m_szServiceName, "%s%s", pszSimpleName, "Service");
	
	sprintf(m_szConfigureFileName, "./%s.xml",m_szServiceName);

//	AddFilePath(m_szConfigureFileName);
	
	__LOGINIT__;
	return true;
}

bool GlobalEnd()
{
	memset(m_szServiceName, 0, sizeof(m_szServiceName));
	memset(m_szConfigureFileName, 0, sizeof(m_szConfigureFileName));
	return true;
}


bool GetWorkPath(char* pszWorkPath)
{
	char szTmp[MAX_PATH] = { 0 };
	
	getcwd(szTmp, MAX_PATH);
	
	strcpy(pszWorkPath, szTmp);
	return true;
}

bool AddFilePath(char* pszFileName)
{
	if(pszFileName == nullptr)
		return false;
	char szTmp[MAX_PATH] = { 0 };
	getcwd(szTmp, MAX_PATH);
	
	char szFileName[MAX_PATH] = { 0 };
	strcpy(szFileName, pszFileName);
	sprintf(pszFileName, "%s/%s", szTmp, szFileName);

	return true;
}

//"192.168.1.1" =>  unsigned int 
unsigned int IPString2Dword(const char* pszIP)
{
	if(nullptr == pszIP || 0 == strlen(pszIP))
		return 0;
	//TODO 
}

//unsigned int  => "192.168.1.0"
char* IPDWORD2String(char* pszIP, unsigned int dwIP)
{
	if(nullptr == pszIP)
		return nullptr;
	//TODO 
}

// net unsigned int => "192.168.1.0"
char* IPNetDWORD2String(char* pszIP, unsigned int dwNetIP)
{
	if(nullptr == pszIP)
		return nullptr;
	//TODO 
}

//"192.168.1.0" => net unsigned int 
unsigned int IPNetString2DWORD(const char* pszIP)
{
	if(nullptr == pszIP || 0 == strlen(pszIP))
		return 0;
	//TODO 
}

//获取本地IPlist
bool GetLocalIPList(std::vector<std::string> &listIP)
{
	//TODO 
	return true;
}
