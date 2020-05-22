#include "CppUrl.h"



int main()
{
	CppUrl curl;
	std::string str;
	curl.HttpsGet("www.baidu.com", str);
	
	printf("%s", str.c_str());

	return 0;
}
