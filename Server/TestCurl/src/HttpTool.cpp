#include "HttpTool.h"
#include <string.h> 


CHttpTool::CHttpTool()
{
}

CHttpTool::~CHttpTool()
{
}




//
size_t CHttpTool::OnReceiveData(void* pContents, size_t Size, size_t nMemb, void* pStream)
{
	
	std::string* pStr  = (std::string*)pStream; 

	(*pStr).append((char*)pContents, Size * nMemb);
	
	return Size * nMemb;
}
	
size_t CHttpTool::OnWriteData2File(void* pData, size_t Size, size_t nMemb, FILE* pFile)
{
	return fwrite(pData, Size, nMemb, pFile);
}
//文件下载接口
CURLcode CHttpTool::OnDownLoadFile(const char* pszURL, const char* pszFileName)
{
	CURL* pCurl = nullptr;
	CURLcode Ret ;
	FILE* pFile = fopen(pszFileName, "ab+");
	do 
	{
		Ret = curl_global_init(CURL_GLOBAL_ALL);
		if(CURLE_OK != Ret)
		{
			break;			
		}

		pCurl = curl_easy_init();
		if(pCurl)
		{
			Ret = curl_easy_setopt(pCurl, CURLOPT_URL, pszURL);
			if(CURLE_OK != Ret)
			{
				strcpy(szErrorMsg, curl_easy_strerror(Ret));
				break;
			}

			// 
			Ret = curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CHttpTool::OnWriteData2File);
			if(CURLE_OK != Ret)
			{
				break;
			}
			
			Ret = curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, pFile);
			if(CURLE_OK != Ret)
			{
				break;
			}
			//
			Ret = curl_easy_perform(pCurl);
			if(CURLE_OK != Ret)
			{
				break;
			}
		}
	}while(0);
	fclose(pFile);
	curl_easy_cleanup(pCurl);
	curl_global_cleanup();
	return Ret;
}
//http Get 请求 
CURLcode CHttpTool::OnHttpGet(const std::string & strUrl, std::string& strResponse, int nTimeOut)
{
	
	CURL* pCurl = curl_easy_init();
	if(nullptr == pCurl)
	{
		return CURLE_FAILED_INIT;
	}
	curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, nTimeOut);
	curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CHttpTool::OnReceiveData);
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (void*) &strResponse);

	CURLcode Ret = curl_easy_perform(pCurl);
	curl_easy_cleanup(pCurl);
	return Ret;
}
//http Post 请求接口 
CURLcode CHttpTool::OnHttpPost(const std::string& strUrl, std::string& strReq, std::string& strResponse, int nTimeOut)
{
	char szData[1024] = { 0 };
	strcpy(szData, strReq.c_str());

	CURL* pCurl = curl_easy_init();
	if(nullptr == pCurl)
	{
		return CURLE_FAILED_INIT;
	}
	
	struct curl_slist* pHeader = nullptr; 
	
	CURLcode Ret = curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
	
	Ret = curl_easy_setopt(pCurl, CURLOPT_POST, 1L);

	pHeader = curl_slist_append(pHeader, "content-type:application/json");

	Ret = curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, szData);

	Ret = curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, nTimeOut);

	Ret = curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CHttpTool::OnReceiveData);

	Ret = curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (void*)& strResponse);

	Ret = curl_easy_perform(pCurl);

	curl_easy_cleanup(pCurl);
	return Ret;
}
	
char*  CHttpTool::GetLastError()
{
	return szErrorMsg;
}
