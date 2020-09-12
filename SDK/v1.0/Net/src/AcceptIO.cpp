#include "AcceptIO.h"
#include "PassiveIO.h"
#include "StreamIO.h"
#include "IOMgr.h"

#include <functional> 

CAcceptIO::CAcceptIO():m_bStop(false)
{
	m_nNewEventType = EPOLLIN ;
}
	
CAcceptIO:: ~CAcceptIO()
{
	
}
	
bool CAcceptIO::IsReadable()
{
	if(m_nEventType & EPOLLIN)
		return true;
	
	return false;
}
	
bool CAcceptIO::OnInit(int nPort, unsigned int dwHostIP/* = 0*/)
{
	int fd = Create();
	if(fd < 0)
	{
		LogError("Create Socket fail.");
		return false;
	}

	int nSwitch = 1;
	CSocket::SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &nSwitch, sizeof(nSwitch));
	
	if(!Bind(nPort))
	{
		return false;
	}
	
	if(!Listen())
	{
		return false;
	}
	
	return true;
}

bool CAcceptIO::OnRecv()
{
	while(IsReadable())
	{
		int fd = CSocket::Accept();
		if(fd <= 0)
		{
			if(EINTR == errno)
				continue;
			else 
			{
				if(EMFILE == errno)
					LogFatal("the file handle is exhaust.");
				
				break;
		
			}
		}

		CStreamIO* pNetClient = new CStreamIO;
		if(nullptr == pNetClient)
		{
			LogError("memory error.");
			return false;
		}
		pNetClient->Attach(fd);
		CPassiveIO* pClient = new CPassiveIO;
		if(nullptr == pClient)
		{
			LogError("memory error.");
			return false;
		}
		//
		int on = 1;
		pNetClient->SetSockOpt(IPPROTO_TCP, TCP_NODELAY, &on, sizeof(int));
		//
		pNetClient->SetOwner(pClient);
		pClient->SetOwner(pNetClient);
		pNetClient->AddRef(__FUNCTION__);
		pClient->Attach(fd);
		struct sockaddr_in sa;
		socklen_t nlen = sizeof(sa);
		pClient->GetPeerName((struct sockaddr*)&sa, &nlen);
		pClient->m_dwIP = ntohl(sa.sin_addr.s_addr);
		pClient->m_nPort = ntohs(sa.sin_port);
		pNetClient->m_dwIP = pClient->m_dwIP;
		pNetClient->m_nPort = pClient->m_nPort;
		pClient->PermitRead();
		g_pIOMgr->AddNetIO((CNetIO*)pClient);
		g_pIOMgr->OnNetConnect(pClient);
	}

	return true;
}

bool CAcceptIO::OnClose()
{	
	ForbidRead();
	OnClose();
	return true;
}
	
void CAcceptIO::Terminate()
{
	OnClose();
	m_bStop = true;
}

void CAcceptIO::ForbidRead()
{
	m_nNewEventType = m_nEventType & ~EPOLLIN;
}

void CAcceptIO::PermitRead()
{
	m_nNewEventType = EPOLLIN;
}
