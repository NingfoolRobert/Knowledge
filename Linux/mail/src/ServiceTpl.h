#pragma once
#include <stdio.h>

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
		if(nullptr != pService)
		{
			pService->Execute();
		}
		else 
		{
			return -1;
		}

		//TODO onTerminate;
		delete pService;
		return 0;
	}
};
