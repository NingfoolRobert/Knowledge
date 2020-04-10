#include "Configure.h"
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <set>




	
CIniConfigure::CIniConfigure()
{
	memset(m_szIniFileName, 0, sizeof(m_szIniFileName));
	m_tLastModTime = 0;
}


CIniConfigure::~CIniConfigure()
{
}



	
bool CIniConfigure::Init(const char* pszConfigureFileName)
{
	if(nullptr == pszConfigureFileName || 0 == strlen(pszConfigureFileName))
		return false;

	FILE* pFile = fopen(pszConfigureFileName, "ab+");
	if(nullptr == pFile)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	unsigned int dwFileSize = ftell(pFile);

	rewind(pFile);
	std::vector<char> buf;
	buf.resize(dwFileSize);

	fread(&buf[0], 1, dwFileSize, pFile);

	return ReadIniFile(&buf[0], dwFileSize);
}

bool CIniConfigure::GetIniString(const char* pszSec, const char* pszKey, const char* pszDefault, char* pszValue)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	if(nullptr == pszSec || nullptr == pszKey || pszDefault == nullptr || nullptr == pszValue)
		return false;

	strcpy(pszValue, pszDefault);
	for(auto i = 0u; i < m_listIniConf.size(); ++i)
	{
		for(auto j = 0u; j < m_listIniConf[i]->size(); ++j)
		{
			auto pEntry = m_listIniConf[i]->at(j);
			if(nullptr == pEntry || strcmp(pszSec, pEntry->szSec) != 0)
				break;
			if(strcmp(pEntry->szEntry, pszKey) == 0)
			{
				strcpy(pszValue, pEntry->szValue);
				return true;
			}
		}
	}

	return true;
//	auto itSec  = m_listConfInfo.find(pszSec);
//	if(itSec == m_listConfInfo.end())
//		strcpy(pszValue, pszDefault);
//	else 
//	{
//		auto pSec = itSec->second;
//		if(nullptr == pSec)
//		{
//			strcpy(pszValue, pszDefault);
//		}
//		else 
//		{
//			auto it = pSec->find(pszKey);
//			if(it == pSec->end())
//				strcpy(pszValue, pszDefault);
//			else 
//			{
//				auto pEntry = it->second;
//				if(nullptr == pEntry)
//					strcpy(pszValue, pszDefault);
//				else 
//					strcpy(pszValue, pEntry->szValue);
//			}
//				
//		}
//		
//	}
	return true;
}

int	CIniConfigure::GetIniInt(const char* pszSec, const char* pszKey, const int nDefault/* = 0*/)
{
	char szValue[256] = { 0 };
	char szDefault[256] = { 0 };
	sprintf(szDefault, "%d", nDefault);
	if(!GetIniString(pszSec, pszKey, szDefault, szValue))
	{
		return  nDefault;
	}

	return atoi(szValue);
}


double	CIniConfigure::GetIniDouble(const char* pszSec, const char* pszKey, const double dDefault/* = 0.0*/)
{
	char szValue[256] = { 0 };
	char szDefault[256] = { 0 };
	sprintf(szDefault, "%lf", dDefault);
	if(!GetIniString(pszSec, pszKey, szDefault, szValue))
	{
		return  dDefault;
	}
	
	return atof(szValue);
}

int64_t	CIniConfigure::GetIniInt64(const char* pszSec, const char* pszKey, const int64_t  llDefault/* = 0*/)
{
	char szValue[256] = { 0 };
	char szDefault[256] = { 0 };
	sprintf(szDefault, "%ld", llDefault);
	if(!GetIniString(pszSec, pszKey, szDefault, szValue))
	{
		return  llDefault;
	}
	
	return strtol(szValue, nullptr, 10);
}

bool CIniConfigure::WriteIniString(const char* pszSec, const char* pszKey, const char* pszValue, const char* pszNote/* = nullptr*/)
{
	if(nullptr == pszSec || nullptr == pszKey || nullptr == pszValue)
		return false;

	std::unique_lock<std::mutex> locker(m_clsLock);
	bool bFindSec = false;
	bool bFindKey = false;
	for(auto i = 0u; i < m_listIniConf.size(); ++i)
	{
		for(auto j= 0u; j < m_listIniConf[i]->size(); ++j)
		{
			auto pEntry =  m_listIniConf[i]->at(j);
			if(nullptr == pEntry || strcmp(pEntry->szSec, pszSec) != 0)
				break;
			if(strcmp(pEntry->szEntry, pszKey) == 0)
			{
				if(strcmp(pEntry->szValue, pszValue) == 0)
					return true;
				bFindKey = true;
				strcpy(pEntry->szValue, pszValue);	
				break;
			}	
			bFindSec = true;
		}
		if(bFindKey)
			break;
		if(bFindSec)
		{
			auto pSec = m_listIniConf[i];
			auto pEntry = std::make_shared<INIENTRYINFO>();
			if(nullptr == pEntry)
				return false;
			strcpy(pEntry->szEntry, pszKey);
			strcpy(pEntry->szValue, pszValue);
			strcpy(pEntry->szSec, pszSec);
			if(pszNote)
			{
				strcpy(pEntry->szNote, pszNote);
			}
			pSec->push_back(pEntry);
			break;
		}
	}
	if(!bFindSec)
	{
		auto pSec = new std::vector<INIENTRYINFOPtr>();
		if(pSec == nullptr)
			return false;
		
		auto pEntry = std::make_shared<INIENTRYINFO>();
		if(nullptr == pEntry)
			return false;
		strcpy(pEntry->szEntry, pszKey);
		strcpy(pEntry->szValue, pszValue);
		strcpy(pEntry->szSec, pszSec);
		if(pszNote)
		{
			strcpy(pEntry->szNote, pszNote);
		}
		pSec->push_back(pEntry);
		m_listIniConf.push_back(pSec);
	}

	return WirteIniFile(); 
}

bool CIniConfigure::WriteIniInt(const char* pszSec, const char* pszKey, const int nValue/* = 0*/, const char* pszNote/* = nullptr*/)
{
	char szValue[128] = { 0 };
	sprintf(szValue, "%d", nValue);
	return WriteIniString(pszSec, pszKey, szValue, pszNote);
}

bool CIniConfigure::WirteIniDouble(const char* pszSec, const char* pszKey, const double dValue/* = 0.0*/, const char* pszNote/* = nullptr*/)
{
	char szValue[128] = { 0 };
	sprintf(szValue, "%lf", dValue);
	return WriteIniString(pszSec, pszKey, szValue, pszNote);
}

bool CIniConfigure::WriteIniInt64(const char* pszSec, const char* pszKey, const int64_t llValue/* = 0*/, const char* pszNote/* = nullptr*/)
{
	char szValue[128] = { 0 };
	sprintf(szValue, "%ld", llValue);
	return WriteIniString(pszSec, pszKey, szValue, pszNote);
}
	
bool CIniConfigure::DelIniSecEntry(const char* pszSec, const char* pszKey)
{
	return true;
}

bool CIniConfigure::DelIniSec(const char* pszSec)
{
	return true;
}
	
bool CIniConfigure::ReadIniFile(const char* pszFileTxt, unsigned int dwFileSize)
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	if(nullptr == pszFileTxt ||  0 == dwFileSize)
		return false;

	std::set<std::string> listTmp;
	char szSection[128] = { 0 };
	const char* pSplite = "\n";
	char* pSave = nullptr;
	
	char* pHead = strtok_r(const_cast<char*>(pszFileTxt), pSplite, &pSave);

	std::vector<INIENTRYINFOPtr>* pSec = nullptr;
	std::string strNote;
	std::vector<char> buf;
	while(pHead)
	{
		int nLen = strlen(pHead);
		buf.resize(nLen + 1);
		strncpy(&buf[0], pHead, nLen + 1);
		
		char* ptr = &buf[0];
		
		while(*ptr == '\t' || *ptr == ' ') ptr++;
		char* pEnd = &buf[0] + nLen;
		while(*pEnd == '\t' || *pEnd == ' ') ptr--;
		++pEnd;
		*pEnd = 0;

		if(*ptr == ';')
		{
			 strNote += ptr;
		}
		else if(*ptr == '[')
		{
			pSec = nullptr;
			++ptr;
			char *pEnd = strchr(ptr, ']');
			if(pEnd)
			{
				strncpy(szSection, ptr, pEnd - ptr);
				if(listTmp.count(szSection) > 0)
				{
					return false;
				}
				
				pSec = new std::vector<INIENTRYINFOPtr>();
				if(nullptr == pSec)
				{
					return false;
				}
				m_listIniConf.push_back(pSec);
			}
		}
		else 
		{
			auto pEntry = std::make_shared<INIENTRYINFO>(); 
			if(nullptr == pEntry)
			{
				return false;
			}

			char* pValue  = strchr(ptr, '=');
			if(nullptr != pValue)
			{
				strcpy(pEntry->szSec, szSection);
				char *pEnd = (pValue - 1);
				while(*pEnd == '\t' || *pEnd == ' ') pEnd--;
				pEnd++;*pEnd = 0;
				strncpy(pEntry->szEntry, ptr, pEnd - ptr);
			
				++pValue;
				while(*pValue == '\t' || ' ' == *pValue) pValue++;
				strcpy(pEntry->szValue, pValue);
		
				strcpy(pEntry->szNote, strNote.c_str());
				strNote = "";
				pSec->push_back(pEntry);
			}

		}
		
		pHead = strtok_r(NULL, pSplite, &pSave);
	}
	
	return true;
}

bool CIniConfigure::WirteIniFile()
{
	int fd = open(m_szIniFileName, O_TRUNC | O_APPEND | O_WRONLY, 0644);
	if(fd < 0)
	{
		return false;
	}
	
	char szTmp[1024] = { 0 };
	INIENTRYINFOPtr pEntry = nullptr;
	for(auto i = 0u; i < m_listIniConf.size(); ++i)
	{
		for(auto j = 0u; j < m_listIniConf[i]->size(); ++j)
		{
			pEntry = m_listIniConf[i]->at(j);
			if(j == 0)
			{
				sprintf(szTmp, "[%s]\n", pEntry->szSec);
				//Write to File 
				write(fd, szTmp, strlen(szTmp));
			}
			if(0 == strlen(pEntry->szNote))
				sprintf(szTmp, "%s=%s\n",pEntry->szEntry, pEntry->szValue);
			else 
				sprintf(szTmp, "%s\n%s=%s\n", pEntry->szNote, pEntry->szEntry, pEntry->szValue);
				
			write(fd, szTmp, strlen(szTmp));
		}
	}

	close(fd);	
	return true;
}
