#include "UserObject.h"
//#include "NetClient.h"


CUserObject::CUserObject(void):m_pNetClient(nullptr),m_nPort(0),m_dwIP(0)
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
    //TODO
    return true;
}

bool CUserObject::SendZipMsg(PHEADER pHeader)
{
    //TODO
    return true;
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