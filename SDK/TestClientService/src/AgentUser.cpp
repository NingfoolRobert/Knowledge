#include "AgentUser.h"
CAgentUser::CAgentUser()
{

}
CAgentUser::~CAgentUser()
{

}
bool CAgentUser::OnMsg(PHEADER pHeader)
{
	LogInfo("%s", __FUNCTION__);
	if(nullptr == pHeader)
	{
		return false;
	}
	switch(pHeader->dwType)
	{
		case 1:
			OnRecvData(pHeader);
			break;
		default:
			break;
	}
	return true;
}

void CAgentUser::OnConnect()
{
	
	LogInfo("%s", __FUNCTION__);
}

void CAgentUser::OnSencondIdle()
{
	LogInfo("%s", __FUNCTION__);

}

void CAgentUser::OnClose()
{
	LogInfo("%s", __FUNCTION__);

}

bool CAgentUser::OnRecvData(PHEADER pHeader)
{
	LogInfo("%s", __FUNCTION__);
	return true;
}
