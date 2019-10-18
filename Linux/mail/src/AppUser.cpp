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
	
//TODD  Parse PB 
	g_ciccAlarmService->SendWarningInfo(buf);
	return true;
}
