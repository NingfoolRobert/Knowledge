

#include "Notify.h"
#include <fstream>
#include <ostream>

int compNotify(const void* arg1, const void* arg2)
{
	CNotify* pNotify1 = (CNotify*) arg1;
	CNotify* pNotify2 = (CNotify*)arg2;
	
	return (pNotify1->GetNotifyType() - pNotify2->GetNotifyType());
}


CNotify::CNotify()
{
	m_pSocket = nullptr;
}

CNotify::~CNotify()
{
	
}


bool CNotify::OnInitialUpdate(const char* pszConfigFileName)
{
	if(pszConfigFileName == nullptr)
	{
		return false;
	}


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


bool CNotify::Notify(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle, CBuffer* pBuffer)
{
	if(nullptr == pBuffer)
		return false;
	return true;
}
  

bool CNotify::OnTimeout(struct tm* pTime)
{	
	return true;
}


bool CNotify::Connect(const char* pszAddr, int nPort)
{
	int nfd = m_pSocket->Create();
	if(nfd <= 0)
	{
		return false;
	}
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

int  CNotify::RecvMsg(char* pszBuf, int nBufLen)
{
	return m_pSocket->Recv(pszBuf, nBufLen);
}

void CNotify::Close()
{
	m_pSocket->Close();
}
