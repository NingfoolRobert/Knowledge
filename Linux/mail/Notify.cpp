

#include "Notify.h"

CNotify::CNotify()
{
	m_pSocket = nullptr;
}

CNotify::~CNotify()
{
	
}


bool CNotify::OnIntialUpdate(const char* pszConfigFileName)
{
	

	//初始化网络
	m_pSocket = new CSocket();
	if(m_pSocket == nullptr)
	{
		return true;
	}

	if(!m_pSocket->Create())
	{
		return false;
	}
	return true;
}

bool CNotify::OnTimeout(struct tm* pTime)
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

bool CNotify::RecvMsg(char* pszBuf, int nBufLen)
{
	m_pSocket->Recv(pszBuf, nBufLen);
	return true;
}
