#include "UserObject.h"



CUserObject::CUserObject():m_tBreak(0), m_tConnection(0), m_dwIP(0), m_nPort(0),m_pNetClient(nullptr)
{

}

CUserObject::~CUserObject()
{

}


bool CUserObject::OnMsg(PHEADER pMsg)
{
	return true;
}

void CUserObject::OnTickCnt()
{
	
}

bool CUserObject::OnBreak()
{
	m_tBreak = time(NULL);
	return true;
}

bool CUserObject::OnConnect()
{
	m_tConnection = time(NULL);
	return true;
}

void CUserObject::BindNetClient(CNetClient* pNetClient)
{
	m_pNetClient = pNetClient;
	if(pNetClient)
	{
		m_dwIP	= m_pNetClient->m_dwIP;
		m_nPort = m_pNetClient->m_nPort;
	}
}

bool CUserObject::IsBindNetClient()
{
	return m_pNetClient != nullptr;
}


bool CUserObject::SendMsg(PHEADER pMsg)
{
	if(IsBindNetClient() && !m_tBreak)
	{
		return m_pNetClient->SendMsg(pMsg);
	}

	return false;
}
	
bool CUserObject::SendMsg(PHEADER pMsg, bool bAsyncSend/* = true*/)
{
	return true;
}

bool CUserObject::SendMsg(CBuffer* pBuffer, bool bAsyncSend/* = true*/)
{
	return true;
}
	
bool CUserObject::SendZipMsg(PHEADER pMsg, bool bAsyncSend/* = true*/)
{
	return true;
}
	
bool CUserObject::ZipMsg(PHEADER pMsg, CBuffer* pBuffer)
{
	return true;
}
	
bool CUserObject::UnZipMsg(PHEADER pMsg,	CBuffer* pBuffer)
{
	return true;
}
