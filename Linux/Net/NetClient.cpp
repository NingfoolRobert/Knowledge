#include "NetClient.h"
#include "UserObject.h"

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

bool CNetClient::OnMsg(PHEADER pMsg)
{
	if(nullptr == m_pUserObject)
		return false;
	m_pUserObject->OnMsg(pMsg);
	return true;
}

bool CNetClient::OnConnect()
{
	if(nullptr == m_pUserObject)
		return false;
	m_pUserObject->OnConnect();
	return true;
}
bool CNetClient::OnBreak()
{
	if(nullptr == m_pUserObject)
		return false;
	m_pUserObject->OnBreak();
	return true;
}
bool CNetClient::OnTickCount()
{
	if(nullptr == m_pUserObject)
		return false;
	m_pUserObject->OnTickCount();
	return true;
}

inline void CNetClient::SetMsgOrigine(unsigned short usMsgOrigine)
{
	m_uMsgOrigin = usMsgOrigine;
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
