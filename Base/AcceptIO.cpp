
//#include "AcceptIO.h"
#include "NetClient.h"
#include "NetService.h"
#include "Log.h"


CAcceptIO::CAcceptIO()
{
	m_pNetService = nullptr;
	m_nNewEvent = EPOLLIN | EPOLLET;
}

CAcceptIO:: ~CAcceptIO()
{

}

bool CAcceptIO::OnInitialUpdate(CNetService* pNetService)
{
	if(pNetService == nullptr)
	{
		LogFatal("%s(%d) Init Appcept IO fail.", __FILE__, __LINE__);
		return false;
	}
	//初始化监听端口 
	if(!Socket(pNetService->m_nPort))
	{
		return false;
	}
	return true;
}

void CAcceptIO::OnRecv() 
{
	// Add Recv new Socket task to thread pool 
	;
}

bool CAcceptIO::Socket(int nPort, unsigned int dwIP /*= 0*/, int nDomain/* = AF_INET*/, int nSockType/* = SOCK_STREAM*/, int nProtocol/* = IPPROTO_IP*/)
{
	if(CNetIO::Create(nDomain, nSockType, nProtocol) <= 0)
	{
		LogFatal("%s(%d) Create Listen Socket fail.", __FILE__, __LINE__);
		return false;
	}
	//
	if(!CNetIO::Bind(nPort, dwIP))
	{
		LogFatal("%s(%d) Bind Socket fail.", __FILE__, __LINE__);
		return false;
	}
	//
	if(!CNetIO::Listen())
	{
		LogFatal("%s(%d) Listen Socket fail.", __FILE__, __LINE__);
		return false;
	}
	
	CNetIO::SetBlockMode();
	return true;
}

void CAcceptIO::AcceptThread()
{
	while(true)
	{
		int fd = CNetIO::Accept();
		if(fd < 0)
		{
			if(errno == EINTR)
			{
				continue;
			}
			else if(errno == EAGAIN)
			{
				return ;
			}
			else if(errno == ECONNABORTED)
			{
				LogError("%s(%d) Connnect break off. ", __FILE__, __LINE__);
				
				return ;
			}
			else if(errno == EMFILE || errno == ENFILE)
			{
				LogFatal("%s(%d) [%s] Open file handle limits err: %d.", __FILE__, __LINE__, __FUNCTION__, errno);
				return ;
			}
			else
			{
				LogError("Listen Socket Error: %d", errno);
				return ;
			}
			
		}
		else 
		{
			CNetClient*  pNetClient = m_pNetService->CreateNetClient();
			if(pNetClient == nullptr)
				continue;	
			pNetClient->OnInitialUpdate(m_pNetService, fd);
			AddNetClient(pNetClient);
		}
	}
}


bool  CAcceptIO::AddNetClient(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
	{
		LogError("%s(%d) Add NetClient is NULL.", __FILE__, __LINE__);
		return false;
	}
	if(nullptr == m_pNetService)
	{
		LogError("%s(%d) NetService is nullptr.", __FILE__, __LINE__);
		return false;
	}
	
	return m_pNetService->AddNetClient(pNetClient);
}
