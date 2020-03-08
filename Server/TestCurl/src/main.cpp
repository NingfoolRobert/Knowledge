


#include "curl/curl.h"
#include "zlib.h"


#include <iostream> 
#include <stdlib.h> 
#include <stdio.h> 


size_t ProcessData(void* pBuffer, size_t size, size_t nmenb, void* pUser)
{
	FILE * fp= (FILE* )pUser;
	size_t nSize = fwrite(pBuffer, size, nmenb, fp);

	return nSize;
}

int main()

{
	CURL *pCurl;
	CURLcode  Ret;
	Ret = curl_global_init(CURL_GLOBAL_DEFAULT);
	pCurl = curl_easy_init();
	if(pCurl)
	{

		FILE* fp = fopen("data.html", "ab+");
		curl_easy_setopt(pCurl, CURLOPT_URL, "https://example.com/");
	
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &ProcessData);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA , fp);
		Ret = curl_easy_perform(pCurl);
		
		if(Ret != CURLE_OK)
		{
			printf("curl_easy_perfor() fail. failed: %s\n", curl_easy_strerror(Ret));
		}
	
		curl_easy_cleanup(pCurl);
	
	}
	curl_global_cleanup();

	return 0;
}

