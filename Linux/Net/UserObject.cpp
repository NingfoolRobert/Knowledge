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
	if(nullptr == m_pNetClient)
		return false;
	return true;
}

bool CUserObject::OnMsg(PHEADER pMsg)		//网络消息到达接口
{
	return true;
}

bool CUserObject::OnBreak()					//网络连接断开事件接口
{
	if(nullptr == m_pNetClient)
		return false;
	return true;
}

bool CUserObject::OnTickCount()				//网络连接分钟定时调用
{
	if(nullptr == m_pNetClient)
		return false;
	return true;
}

bool CUserObject::SendMsg(PHEADER pMsg)
{
	if(nullptr == pMsg || nullptr == m_pNetClient)
		return false;
	return m_pNetClient->SendMsg(pMsg);
}

void CUserObject::BindNetClient(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
		return;

	m_pNetClient = pNetClient;

	m_nEnterPort = pNetClient->m_nEnterPort;
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

	struct in_addr addr;
	memset(&addr,0, sizeof(struct in_addr));
	addr.s_addr = uUserIP;
	
	strcpy(m_szUserIP,inet_ntoa(addr));


}
void CUserObject::GetWaitMsgCount(int &cnSendWait, int & &cnRecvWait)
{
	
}
