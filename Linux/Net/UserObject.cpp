#include "UserObject.h"


CUserObject::CUserObject()
{
	m_pNetClient = nullptr;
	m_uUserIP  = 0;
	memset(m_szUserIP, 0, sizeof(m_szUserIP));
	m_nEnterPort = 0;
}

CUserObject::~CUserObject();
{
	
}


bool CUserObject::OnInitialUpdate()
{
	return true;
}

bool CUserObject::OnConnect()				//网络建立连接
{
	return true;
}

bool CUserObject::OnMsg(PHEADER pMsg)		//网络消息到达接口
{
	return true;
}

bool CUserObject::OnBreak()					//网络连接断开事件接口
{
	return true;
}

bool CUserObject::OnTickCount()				//网络连接分钟定时调用
{
	return true;
}

bool CUserObject::SendMsg(PHEADER pMsg)
{

	return true;
}

void CUserObject::BindNetClient(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
		return;

	m_pNetClient = pNetClient;
}
inline	CNetClient*	CUserObject::GetNetClient()
{
	return m_pNetClient;
}
bool CUserObject::IsBindNetClient()
{
	if(nullptr == m_pNetClient)
		return false;
	return true;
}

void CUserObject::SetNetUserIP(unsigned int  uUserIP)
{
	m_uUserIP = uUserIP;
}
void CUserObject::GetWaitMsgCount(int &cnSendWait, int & &cnRecvWait)
{
	
}
