#pragma once
#include <stdio.h>

template<class T>
class CServiceTpl
{
public:
	CServiceTpl() = default;
	~CServiceTpl() = default;
public:
	bool Execute(const char* pszServiceName)
	{
		T* pService = new T;
		if(nullptr != pService)
		{
			pService->Execute();
		}
		else 
		{
			return false;
		}
		//TODO onTerminate;
		return true;
	}
};
