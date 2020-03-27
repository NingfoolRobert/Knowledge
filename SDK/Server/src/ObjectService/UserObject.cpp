#include "UserObject.h"
#include "NetClient.h"


CUserObject::CUserObject(void):m_dwIP(0), m_nPort(0), m_pNetClient(nullptr)
{
}

CUserObject::~CUserObject()
{
}

bool CUserObject::OnMsg(PHEADER pHeader)
{
    return true;
}

void CUserObject::OnConnect()
{
}

void CUserObject::OnSecondIdle()
{
}
    
void CUserObject::OnClose()
{
}
  
bool CUserObject::SendMsg(PHEADER pHeader)
{
	if(m_pNetClient)
	{
		return m_pNetClient->SendMsg(pHeader);
	}
	return false;
}

bool CUserObject::SendZipMsg(PHEADER pHeader)
{
    //TODO zip 
	
	CBuffer stBuf;

    if(m_pNetClient)
	{
		return m_pNetClient->SendMsg(&stBuf);
	};

	return false;
}

void CUserObject::BindNetClient(CNetClient* pNetClient)
{
    m_pNetClient = pNetClient;
}
    
bool CUserObject::IsBindNetClient()
{
    return m_pNetClient != nullptr;
}

void CUserObject::SetPeerAddr(unsigned int dwIP, int nPort)
{
    m_dwIP  = dwIP;
    m_nPort = nPort;
}
	
void CUserObject::Terminate()
{
	if(m_pNetClient)
	{
		m_pNetClient->Terminate();
	}
}
