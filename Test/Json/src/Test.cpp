
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


bool Gene(rapidjson::Value& val)
{
	int nName = val["name"].GetInt();
	std::cout << nName << std::endl;
	int nID = val["id"].GetInt();
	std::cout << nID << std::endl;
	return true;
}

int main()
{

	using namespace rapidjson;
	
	char *pszJson = "{ \"name\":\"tom\", \"age\":15,\"class\":[{\"name\":1, \"id\":10}] }";

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

	Value& val = doc["class"];

	if(val.IsArray())
	{
		for(auto i = 0; i < val.Size(); ++i)
		{
			Value& cl = val[i];
			Gene(cl);
		}
	}

	return 0;
}
