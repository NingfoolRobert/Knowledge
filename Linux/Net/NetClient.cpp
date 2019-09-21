#include "NetClient.h"


CNetClient::CNetClient()
{

}

CNetClient::~CNetClient()
{

}


bool CNetClient::SendMsg(PHEADER pMsg)
{
	if(nullptr == pMsg)
		return false;
	//TODO
	return true;
}

void CNetClient::Terminate(bool bWhenSendOver/* = false*/)
{

}

void CNetClient::SetMsgOrigine(unsigned short usMsgOrigine)
{

}

void CNetClient::SetRecvIdleTime(int nSecond)
{
	
}

inline void CNetClient::BindUserObject(CUserObject* pUser)
{
	if(nullptr == pUser)
		return ;
	m_pUserObject = pUser;
}//TODO

inline bool CNetClient::IsBindUserObject()
{
	if(nullptr == m_pUserObject)
		return false;
	return true;
}
