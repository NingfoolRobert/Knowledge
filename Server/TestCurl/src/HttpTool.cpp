#include "HttpTool.h"

CHttpTool::CHttpTool(void)
{
}

CHttpTool::~CHttpTool(void)
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
size_t CHttpTool::OnDownLoadFile(const char* pszURL, const char* pszFileName)
{
	CURL* pCurl = nullptr;
	FILE* pFile = fopen(pszFileName, "ab+");
	do 
	{
		CURLCode Ret = curl_global_init(CURL_GLOBAL_ALL);
		if(CURL_OK != Ret)
		{
			break;			
		}

		pCurl = curl_easy_init();
		if(pCurl)
		{
			Ret = curl_easy_setopt(pCurl, CURLOPT_URL, pszURL);
			if(CURL_OK != Ret)
			{
				strcpy(szErrorMsg, curl_easy_strerror(Ret));
				break;
			}

			// 
			Ret = curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CHttpTool::OnWriteData2File);
			if(CURL_OK != Ret)
			{
				break;
			}
			
			Ret = curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, pFile);
			if(CURL_OK != Ret)
			{
				break;
			}
			//
			Ret = cur_easy_perfrom(pCurl);
			if(CURL_OK != Ret)
			{
				break;
			}
		}
	}while(0);
	fclose(pFile);
	curl_easy_cleanup(pCurl);
	curl_global_cleanup();
	return 0;
}
//http Get 请求 
CURLCode CHttpTool::OnHttpGet(const std::string & strUrl, std::string& strResponse, int nTimeOut)
{
		

	//TODO  
	return 0;
}
//http Post 请求接口 
CURLCode CHttpTool::OnHttpPost(const std::string& strUrl, std::string& strReq, std::string& strResponse, int nTimeOut)
{
	//TODO 
	//
	return 0;
}
	
char*  CHttpTool::GetLastError()
{
	return szErrorMsg;
}
