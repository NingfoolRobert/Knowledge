#include "ConfigHelper.h"
#include "tinyxml2.h"
#include <string>


using namespace std;

ConfigHelper* ConfigHelper::instance = nullptr;

std::string ConfigHelper::GetContentFromConfigFile(std::string cn)
{
	if (cn.length() <= 0)
	{
		return "";
	}
	
	if (map_configContent.size() <= 0)
	{
		return "";
	}
	auto itr = map_configContent.find(cn);
	if (itr == map_configContent.end())
	{
		return "";
	}
	else
	{
		return itr->second;
	}
}

bool ConfigHelper::InitWithRelativePath(string file)
{
	try{
		string strFileName = file;

		if (FILE *file = fopen(strFileName.c_str(), "r"))
		{
			fclose(file);

			//Read the source config
			tinyxml2::XMLDocument doc;
			if(doc.LoadFile(strFileName.c_str()))
			{
				return false;
			}

			tinyxml2::XMLElement* root = doc.FirstChildElement("root");
			if(root == nullptr)
				return false;

			tinyxml2::XMLElement* item = root->FirstChildElement("item");
			if(item == nullptr)
				return false;

			while (item)
			{
				const auto info = item->FirstAttribute();
				map_configContent.insert(make_pair(info->Name(), info->Value()));
				item = item->NextSiblingElement();
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (exception ex)
	{
		return false;
	}
}



