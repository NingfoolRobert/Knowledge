#include "AppUser.h"


CAppUser::CAppUser()
{
}

CAppUser::~CAppUser()
{

}

void CAppUser::ProcessItem(zmsg_t* zMsg)
{

	if(nullptr == zMsg)
		return ;
}



bool CAppUser::ParseWarnningInfo(/*PB::Message* pMessage, */)
{
	char buf[1024] ={0};

	int nLevel = 0;
	std::string strAppType = "";
//TODD  Parse PB 
	g_ciccAlarmService->SendWarningInfo(nLevel, strAppType, buf);
	return true;
}
