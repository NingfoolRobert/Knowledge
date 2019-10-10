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

