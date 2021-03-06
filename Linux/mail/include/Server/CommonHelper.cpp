#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

#include "CommonHelper.h"
#include "tinyxml2.h"

bool CommonHelper::IsDir(const char* pszDirName)
{
	if(pszDirName == nullptr || 0 == strlen(pszDirName))
		return false;
	struct stat sBuf;

	if(stat(pszDirName, &sBuf) != 0)
		return false;
	if(sBuf.st_mode & S_IFDIR)
	{
		return true;
	}
	return false;
	
}
bool CommonHelper::IsFile(const char* pszFileName)
{
	if(pszFileName == nullptr || 0 == strlen(pszFileName))
		return false;
	struct stat sBuf;

	if(stat(pszFileName, &sBuf) != 0)
		return false;
	if(sBuf.st_mode & S_IFREG)
	{
		return true;
	}
	return false;

bool CommonHelper::GetAllFileName(const char* pszPathName, std::vector<std::string>& listFileName, bool bRcv/* = false*/)
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];
    if ((dir=opendir(pszPathName)) == NULL)
	{
		return false;
	}
	while ((ptr=readdir(dir)) != NULL)					
	{					        
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent DIR 
			continue;
		else if(ptr->d_type == 8)    ///file
		{
			sprintf(base, "%s/%s",pszPathName,ptr->d_name);
			listFileName.push_back(base);
		}
		else if(ptr->d_type == 10)    ///link file
		{
			//
		}
		else if(ptr->d_type == 4 && bRcv)    ///dir
		{
			memset(base, 0,sizeof(base));
			sprintf(base, "%s/%s", pszPathName,ptr->d_name);
			GetAllFileName(base, listFileName);
		}
	}
	closedir(dir);
	return true;							
}

bool CommonHelper::Char2Base64(char* pszBuf64, const char* pszSrc,int nLen)
{
	if(pszBuf64 == nullptr || pszSrc == nullptr)
		return false;

	static char Base64Code[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};	//base64 所映射的字符表

	int nPoint = 2;
	
	int nIndex = 0;

	unsigned char n = 0;		//上一源字符的残留值
	for(int i = 0; i < nLen; ++i)
	{
		if(nPoint == 2)
		{
			nIndex = ((*pszSrc) >> nPoint) & 0x3f;		//取得pszSrc 的高nPoint位
		}
		else if(nPoint == 4)
		{
			nIndex = ((*pszSrc) >> nPoint) & 0xf;		//
		}
		else if (nPoint == 6)
		{
			nIndex = ((*pszSrc) >> nPoint) & 0x3;
		}

		nIndex += n;
		*pszBuf64++ = Base64Code[nIndex];
		n = ((*pszSrc) << (6 - nPoint));	//计算字符中的残留值
		n = n & 0x3f;	//确保n的最高位为0;
		nPoint += 2;
		if(nPoint == 8)
		{
			nIndex = (*pszSrc) & 0x3f;		//提取最低6位就是索引
			*pszBuf64++ = Base64Code[nIndex];
			n = 0; 
			nPoint = 2;
		}
		pszSrc++;
	}
	if( n !=0 )
	{
		*pszBuf64++ = Base64Code[n];

	}
	if(nLen % 3 == 2)
	{
		*pszBuf64 = '=';
	}
	else if(nLen % 3 == 1)
	{
		*pszBuf64++ = '=';
		*pszBuf64 = '=';
	}

	return true;
}



bool CommonHelper::GetFileTime(const char* pszFileName, time_t &lCreateTime, time_t& lModifyTime, time_t& lAccessTime)
{
	if(nullptr == pszFileName || 0 == strlen(pszFileName))
		return false;
	FILE* pFile = fopen(pszFileName,"r");
	if(nullptr == pFile)
		return false;
	struct stat buf;
	int fd = fileno(pFile);
	if(fd <= 0)
	{
		fclose(pFile);
		return false;
	}
	fstat(fd, &buf);

	close(fd);
	fclose(pFile);
	lCreateTime = buf.st_ctime;
	lModifyTime = buf.st_mtime;
	lAccessTime = buf.st_atime;

	return true;
}

bool CommonHelper::GetFileInfo(const char* pszFileName, long& lFileSize, time_t& tModify)
{
	if(pszFileName == nullptr)
		return false;
	FILE * pFile = fopen(pszFileName, "r");
	if(nullptr == pFile)
	{
		return false;
	}
	struct stat buf;
	int fd = fileno(pFile);
	if(fd <= 0)
	{
		return false;
	}
	fstat(fd, &buf);
	lFileSize = buf.st_size;
	tModify = buf.st_mtim.tv_sec;
	return true;
}

bool CommonHelper::GetStringXMLConfig(const char* pszSection, const char* pszKey, const char* pszDefaultValue,char* pszValue, const char* pszXMLFileName)
{
	strcpy(pszValue, pszDefaultValue);
	if(pszXMLFileName == nullptr)
		return false;
	using namespace tinyxml2;
	try 
	{
		XMLDocument doc;
		if(doc.LoadFile(pszXMLFileName))
		{
			return false;
		}
		XMLElement* pService = doc.FirstChildElement("Service");
		if(pService == nullptr)
		{
			return false;
		}
		XMLElement* pSection = pService->FirstChildElement(pszSection);
		if(pSection == nullptr)
			return false;
	//	XMLAttribute* pKey = pSection->Attribute(const_cast<char*>(pszKey));
	//	if(pKey == nullptr)
	//		return false;
	//	strcpy(pszValue, pKey->Value());
			
	}
	catch(std::exception e)
	{
		return false;
	}
	return true;
}


time_t CommonHelper::GetFileModifyTime(const char* pszFileName)
{
	time_t tChangeTime;
	time_t tModifyTime;
	time_t tAccessTime;
	if(GetFileTime(pszFileName, tChangeTime, tModifyTime, tAccessTime))
	{
		return tModifyTime;
	}
	return 0;
}


//int64 20191023162536格式转化为2019-10-23 16:25:36
bool CommonHelper::TransferTimeInt2String(char* pszTime, int64_t&  nTime)
{
	if(nullptr == pszTime)
		return false;
	
	char szTmp[64] = { 0 };
	
	sprintf(szTmp, "%ld", nTime);
	
	std::string str(szTmp);
	
	sprintf(pszTime, "%s-%s-%s %s:%s:%s", 
			(str.substr(0,4)).c_str(), 
			(str.substr(4,2)).c_str(),
			(str.substr(6,2)).c_str(), 
			(str.substr(8,2)).c_str(), 
			(str.substr(10,2)).c_str(), 
			(str.substr(12,2)).c_str() 
			);
		return true;

//	//年 
//	int64_t year = nTime/ 10000000000;
//	char szTmp[16] = { 0 };
//	sprintf(szTmp, "ld-", year);
//	strcpy(pszTime, szTmp);
//	int64_t nRedisal  = nTime % 100000000000;
//	int64_t month = nRedisal / 10000000;
//	// 月
//	sprintf(szTmp, "%02ld-", month);
//	strcat(pszTime, szTmp);
//	nRedisal % = 10000000;
//	//日 
//	int  day = nRedisal / 10000
}
