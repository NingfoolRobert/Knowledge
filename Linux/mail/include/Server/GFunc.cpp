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

