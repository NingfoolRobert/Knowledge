#include "AppUser.h"

CAppUser::CAppUser()
{
}
//CAppUser::CAppUser(std::string g, std::string h, std::string t):BaseHandler(g, h, t)

CAppUser::~CAppUser()
{

}

void CAppUser::ProcessItem(zmsg_t* zMsg)
{

	if(nullptr == zMsg)
		return ;

	
}



