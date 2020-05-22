#include "CppUrl.h"
#include "LogFile.h"

#include "string.h"

size_t WriteGetResp(void *buffer, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)buffer, 0, size *nmemb);
	return size * nmemb;
}	

CppUrl::CppUrl()
{
	curl_global_init(CURL_GLOBAL_ALL);
}
	
CppUrl::~CppUrl()
{

}
	

bool CppUrl::HttpsGet(const char* pszUrl, std::string& respdata)
{
	CURL* pcurl  = curl_easy_init();
	if(nullptr == pcurl)
	{
		LogError("init curl fail.");
		return false;
	}

	//std::string respdata;
	curl_easy_setopt(pcurl, CURLOPT_URL, pszUrl);
	curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION,WriteGetResp);
	curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, &respdata);

	bool bCA = false;
	
	if(!bCA)
	{
		curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else 
	{
		curl_easy_setopt(pcurl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(pcurl, CURLOPT_CAINFO, "");
	}

	do 
	{
		CURLcode res = curl_easy_perform(pcurl);
		if(res != CURLE_OK)
		{
			LogError("curl perform fail. %s", curl_easy_strerror(res));
			break;
		}
	}while(0);
	curl_easy_cleanup(pcurl);
	return true;
}
	
bool CppUrl::HttpsPost(const char* pszUrl, std::string& respdata)
{
	

	return true;
}