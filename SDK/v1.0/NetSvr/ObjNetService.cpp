#include "ObjNetService.h"


	
CObjNetService::CObjNetService()
{

}
	
CObjNetService::~CObjNetService()
{

}
	


bool CObjNetService::OnNetMsg(CNetClient* pNetClient, PHEADER pHeader)
{
	if(!pNetClient->IsBindUserObject())	
	{
		CUserObject* pUser = OnCreateUserObject(pHeader->dwType);
		if(pUser)
		{
			pNetClient->BindUserObject(pUser);
			pUser->m_dwIP = pNetClient->m_dwIP;
			pUser->m_nPort = pNetClient->m_nPort;
			pUser->BindNetClient(pNetClient);
			pUser->OnConnect();
		}
	}

	return CNetService::OnNetMsg(pNetClient, pHeader);
}
