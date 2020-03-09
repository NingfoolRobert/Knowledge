#pragma once
#include <stdio.h>
#include "GFunc.h"

int	 __argc = 0;
char** __argv = nullptr;

template<class T>
class CServiceTpl
{
public:
	CServiceTpl() = default;
	~CServiceTpl() = default;
public:
	int Execute(const char* pszServiceName)
	{
		T* pService = new T;
		if(nullptr == pService)
		{
			return -1;
		}
		if(GlobalBegin(pszServiceName))
		{
			pService->Execute(__argv[1]);
		}

		GlobalEnd();
		delete pService;
		return 0;
	}
};
