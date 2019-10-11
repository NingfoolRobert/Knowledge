

#include "Notify.h"




bool CNotify::OnIntialUpdate(const char* pszConfigFileName)
{
	return true;
}

bool CNotify::GetRecvMailAddr(std::vector<std::string>& listRecvMailContactor)
{
	return true;
}

bool CNotify::SendNotify()
{
	return true;
}



bool CNotify::Connect(const char* pszAddr, int nPort)
{
	int nfd = m_pSocket->Create();
	bool nRet = m_pSocket->Connect(pszAddr,nPort);	
	if(!nRet)
	{
		//TODO 
		return false;
	}
	
	return true;
}

bool CNotify::SendNotifyInfo(const char* pszInfo, int nBufLen)
{
	if(m_pSocket->Send(pszInfo, nBufLen))
		return true;
	return false;
}
