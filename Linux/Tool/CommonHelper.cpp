#include "CommonHelper.h"



bool GetAllFileName(const char* pszPathName, std::vector<std::string>& listFileName, bool bRcv/* = false*/)

//int readFileList(char *basePath)
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

bool Char2Base64(char* pszBuf64, const char* pszSrc,int nLen)
{
	if(pszBuf64 == nullptr || pszSrc == nullptr)
		return false;

	static char Base64Code[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};	//base64 所映射的字符表

	int nPoint = 2;
	
	int nIndex = 0;

	char n = 0;		//上一源字符的残留值
	for(int i = 0; i < nLen, ++i)
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
