#pragma once
#include <stdio.h>
#include "GFunc.h"


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
			pService->Execute();
		}

		GlobalEnd();
		delete pService;
		return 0;
	}
};
