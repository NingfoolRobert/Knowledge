#include "UserObject.h"



CUserObject::CUserObject()
{
	m_pNetClient = nullptr;
}

CUserObject::~CUserObject()
{

}


bool CUserObject::OnConnect()		//链接到服务端
{
	return true;
}

bool CUserObject::OnSecondIdle()	//定时调用
{
	return true;
}

bool CUserObject::OnClose()			//客户端关闭
{
	return true;
}

bool CUserObject::SendMsg(PHEADER pMsg)
{
	if(nullptr == m_pNetClient)
		return false;
	return m_pNetClient->SendMsg(PHEADER);
}
	
bool CUserObject::SendZipMsg(PHEADER pMsg)
{
	if(nullptr == m_pNetClient)
		return false;
	return m_pNetClient->SendZipMsg(pMsg);
}
