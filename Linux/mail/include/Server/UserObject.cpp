
#include "UserObject.h"

CUserObject::CUserObject()
{

}

CUserObject::~CUserObject()
{

}

bool CUserObject::OnMsg(const char* pszMsg)
{
	if(nullptr == pszMsg)
		return false;
	return true;
}

