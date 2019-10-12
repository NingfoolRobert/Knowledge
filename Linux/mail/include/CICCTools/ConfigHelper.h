#ifndef CONFIG_HELPER
#define CONFIG_HELPER

#include <cstdint>
#include <string>
#include "MappingHandle.h"

#define MYCONFIG (*ConfigHelper::GetInstance())

class ConfigHelper
{

public:

	static ConfigHelper* GetInstance(){
		if (nullptr == instance)
			instance = new ConfigHelper();
		return instance;
	}

	bool InitWithRelativePath(std::string file);

	std::string GetContentFromConfigFile(std::string);

private:

	static ConfigHelper *instance;
	ConfigHelper(){};
	ConfigHelper(const ConfigHelper &);
	ConfigHelper& operator = (const ConfigHelper &);

	hash_map<std::string, std::string> map_configContent;
};

#endif



