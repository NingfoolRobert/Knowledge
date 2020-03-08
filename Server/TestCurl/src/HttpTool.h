/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxHttpTool.h
 * @brief:xxxHttpTool 
 * @author:xxxHttpToolnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-03-07
 */

#pragma once 

#include <curl.h> 
#include <zlib.h>  

#include <iostream> 
#include <vector> 
#include <string> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 


class CHttpTool 
{
public: 
	CHttpTool();
	~CHttpTool();
public:
	//
	static size_t OnReceiveData(void* pContents, size_t Size, size_t nMemb, void* pStream);
	
	static size_t OnWriteData2File(void* pData, size_t Size, size_t nMemb, FILE* pFile);
	//文件下载接口
	CURLcode OnDownLoadFile(const char* pszURL, const char* pszFileName);
	//http Get 请求 
	CURLcode  OnHttpGet(const std::string & strUrl, std::string& strResponse, int nTimeOut);
	//http Post 请求接口 
	CURLcode  OnHttpPost(const std::string& strUrl, std::string& strReq, std::string& strResponse, int nTimeOut);

	

public:
	char*  GetLastError();

private:
	char		szErrorMsg[256];
	
};
