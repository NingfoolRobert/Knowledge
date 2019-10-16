#pragma once

#include "czmq.h"
#include "Log.h"

#include <string>
#include <set>




class CSubClient
{
public:
	CSubClient();
	virtual ~CSubClient();
public:

	bool	Init();
	bool	OnMsg(zmg_t* m);
	bool	OnTimeOut(struct tm* pTime);
	
private:

	std::string			  m_strSubInfo;
	std::set<std::string> m_listFilter;
};
