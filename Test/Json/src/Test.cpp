
//#include <json/writer.h>
//#include <json/stringbuffer.h>
//#include <json/document.h>
#include <writer.h>
#include <stringbuffer.h>
#include <document.h>

#include <iostream> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string> 



int main()
{

	using namespace rapidjson;
	
	char *pszJson = "{ \"name\":\"tom\", \"age\":15 }";

	Document doc;
	if(doc.Parse<0>(pszJson).HasParseError())
	{
		printf("error parse");
		return 0;
	}

	const	char* pstr = doc["name"].GetString();
	std::cout << pstr << std::endl;
	
	int nAge = doc["age"].GetInt();
	std::cout << nAge << std::endl;
	

	return 0;
}
