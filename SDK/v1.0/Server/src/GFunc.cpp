#include "GFunc.h"
#include <string.h>
#include "BufferMgr.h"
#include "LogFile.h"

#include <unistd.h>
//#include "Log.h"
#include <arpa/inet.h>


bool GlobalBegin(const char* pszSimpleName)
{
	if(pszSimpleName == nullptr || 0 == strlen(pszSimpleName))
		return false;
	sprintf(m_szServiceName, "%s%s", pszSimpleName, "Service");
	
	sprintf(m_szConfigureFileName, "./%s.xml",m_szServiceName);
	
	sprintf(m_szServiceLogFileName, "%s.log", m_szServiceName);
	AddFilePath(m_szServiceLogFileName);
	
//	__LOGINIT__;
	Singleton<CLogFile>::GetInstance().OnInitialUpdate(m_szServiceLogFileName);	

	CBufferMgr* pBufMgr = new CBufferMgr;
	if(pBufMgr ==  nullptr)
	{
		LogError("Memory Data error.");
		return false;
	}

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

//"192.168.1.1" =>  unsigned int (主机)
unsigned int IPStr2Host(const char* pszIP)
{
	if(nullptr == pszIP || 0 == strlen(pszIP))
		return 0;
	
	char *ps = strdup(pszIP);
	char delim[] =".";
	char *token;
	unsigned int dwHostIP = 0;
	for(token = strsep(&ps, delim); token != nullptr; token =strsep(&ps,delim))
	{
		dwHostIP <<= 8;
		dwHostIP += atoi(token);
	}
	
	return dwHostIP;
	
	
//	struct in_addr addr;
//	int nRet = inet_aton(pszIP, &addr);
//	if(!nRet)
//	{
//		return 0;
//	}
//	return ntohl(addr.s_addr);
}

//unsigned int(主机)  => "192.168.1.0"
char* IPHost2Str(char* pszIP, unsigned int dwHostIP)
{
	if(nullptr == pszIP)
		return nullptr;
	sprintf(pszIP, "%d.%d.%d.%d", dwHostIP >> 24, (dwHostIP & 0xff0000) >> 16, (dwHostIP & 0xff00) >> 8, dwHostIP & 0xff);	
	
	return pszIP;
}

// net unsigned int => "192.168.1.0"
char* IPNet2Str(char* pszIP, unsigned int dwNetIP)
{
	if(nullptr == pszIP)
		return nullptr;
	struct in_addr addr;
	addr.s_addr = dwNetIP;
	strcpy(pszIP, inet_ntoa(addr));
	return pszIP;
}

//"192.168.1.0" => net unsigned int 
unsigned int IPStr2Net(const char* pszIP)
{
	if(nullptr == pszIP || 0 == strlen(pszIP))
		return 0;
	
	struct in_addr addr;
	int nRet = inet_aton(pszIP, &addr);
	if(!nRet)
		return 0;
	return (unsigned int)addr.s_addr;
}

//获取本地IPlist
bool GetLocalIPList(std::vector<std::string> &listIP)
{
	//TODO 
	return true;
}

//sockaddr, 获取IP：Port 
bool GetIPAndPort(unsigned int &dwHostIP, int& nPort, struct sockaddr_in* pSockAddr)
{
	if(nullptr == pSockAddr)
	{
		return false;
	}
	
	nPort = ntohs(pSockAddr->sin_port);
	dwHostIP = ntohl(pSockAddr->sin_addr.s_addr);
	return true;
}

bool HostIP2Str(unsigned int dwHostIP, char* pszIP)
{
	if(pszIP == nullptr)
		return false;
	struct in_addr addr;
	unsigned int dwNetIP = htonl(dwHostIP);
	memcpy(&addr, &dwNetIP, sizeof(unsigned int));
	strcpy(pszIP, inet_ntoa(addr));
	
//	int nFirst  = dwHostIP & 0x1000;
//	int nSecond = dwHostIP & 0x0100);
//	int nThird = dwHostIP & 0x0010;
//	int nForth = dwHostIP & 0x0001;
//	sprintf(pszIP, "%d.%d.%d.%d", nFirst>>3, nSecond >> 2, nThird >> 1, nForth);
	return true;
}

bool Compress(char* pszDst, int nDstLen, const char* pszSrc, int nSrcLen)
{
	return true;
}

bool UnCompress(char* pszDst, int nDstLen, const char* pszSrc, int nSrcLen)
{
	return true;
}

unsigned int String2HostIP(const char* pszIP)
{
	if(nullptr == pszIP)
	{
		return 0;
	}
	char *ps = strdup(pszIP);
	char delim[] =".";
	char *token;
	unsigned int dwHostIP = 0;
	for(token = strsep(&ps, delim); token != nullptr; token =strsep(&ps,delim))
	{
		dwHostIP <<= 8;
		dwHostIP += atoi(token);
	}
	
	return dwHostIP;
}

bool GetXMLConfigString(const char* pszNode, const char* pszAttr, const char* pszDefault,  char* pszValue)
{
	if(nullptr == pszNode || nullptr == pszAttr || nullptr == pszValue || nullptr == pszDefault)
		return false;
	return true;
}

bool GetDirPath(const char* pszAllFileName, char* pszAllDirPath)
{
	if(pszAllFileName == nullptr  || nullptr == pszAllDirPath)
		return false;
	
	char* pEnd = strrchr(const_cast<char*>(pszAllFileName), '/');
	if(nullptr == pEnd) return false;
	
	strncpy(pszAllDirPath,  pszAllFileName, pEnd - pszAllFileName);
	return true;
}

bool CreateAllPath(const char* pszAllDir)
{
	if(nullptr == pszAllDir)
		return false;

	char szTmp[256] = { 0 };
	strcpy(szTmp, pszAllDir);

	int nLen = strlen(pszAllDir);
	if('/' != szTmp[nLen - 1])
	{
		strcat(szTmp, "/");
		nLen++;
	}
//	
	for(int  i = 1; i < nLen; ++i)
	{
		if('/' == szTmp[i])
		{
			szTmp[i] = 0;
			if(access(szTmp, F_OK) == -1)
			{
				if(!mkdir(szTmp, 0777) == -1)
				{
					return false;
				}
			}
			szTmp[i] = '/';
		}

	}

	return true;
}

bool GetCurrentPath(char* pszDirName)
{
	if(nullptr == pszDirName)
		return false; 
	//TDO 

	getcwd(pszDirName, 256);

	return true;
}

bool GetXMLAttrString(rapidxml::xml_node<>* pNode,  const char* pszEntry, const char* pszDefault, char* pszValue)
{
	if(nullptr == pNode || nullptr == pszEntry || nullptr == pszValue)
		return false;
	using namespace rapidxml;
	
	xml_attribute<>* pAttr = pNode->first_attribute(pszEntry);
	if(nullptr == pAttr)
	{
		strcpy(pszValue, pszDefault);
		return true;
	}
	
	strcpy(pszValue, pAttr->value());


	return true;
}

int  GetXMLAttrInt(rapidxml::xml_node<>* pNode, const char* pszEntry, const int nDefault)
{
	char szTmp[32] = { 0 };
	if(!GetXMLAttrString(pNode, pszEntry, "", szTmp))
		return nDefault;

	if(szTmp[0] == 0)
		return nDefault;
	
	return atoi(szTmp);
}

time_t GetFileModTime(const char* pszFileName)
{
	if(nullptr == pszFileName)
		return 0;

	struct stat buf;
	
	int nRet = stat(pszFileName, &buf);
	if(nRet != 0)
		return 0;
	
	return static_cast<time_t>(buf.st_mtime);
}
