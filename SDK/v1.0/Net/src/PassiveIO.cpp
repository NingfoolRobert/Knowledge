#include "PassiveIO.h"
#include "IOMgr.h"

	
CPassiveIO::CPassiveIO(): m_tLastMsg(time(NULL)),m_tBreak(0)
{
	m_fd = 0;
}

CPassiveIO::~CPassiveIO()
{
	
}

	
bool CPassiveIO::OnRecv()
{
	if(m_pOwner)
		return m_pOwner->OnRecv();
	return false;
}

bool CPassiveIO::OnClose()
{
	m_tBreak = time(NULL);
	OnBreak();

	m_nNewEventType = EPOLL_EVENT_TYPE_CLOSE;
	return true;
}

bool CPassiveIO::OnMsg(PHEADER& pMsg)
{
	m_tLastMsg = time(NULL);
	AddRef();
	g_pIOMgr->OnNetMsg(this, pMsg);	
	Release();

	return true;
}
	
void CPassiveIO::OnBreak()
{
	AddRef();
	g_pIOMgr->OnNetBreak(this);	
	Release();
}
	
void CPassiveIO::OnTickCnt()
{
	AddRef();
	g_pIOMgr->OnNetTick(this);
	Release();
}
	
void CPassiveIO::Terminate()
{
	m_pOwner->OnClose();
}
	
bool CPassiveIO::SendMsg(PHEADER pMsg)
{
	std::unique_lock<std::mutex>	locker(m_clsSendLock);
	if(nullptr == pMsg)
		return false;
	if(m_tBreak) return false;

	unsigned int dwDataLen = pMsg->dwLength + sizeof(HEADER);
	pMsg->dwLength = htonl(pMsg->dwLength);		//
	unsigned int dwSended = 0;
	while(true)
	{
		int nSend = Send((char*)pMsg + dwSended,  dwDataLen - dwSended);
		if(nSend <= 0)
		{
			if(EINTR == errno || EAGAIN == errno)
				continue;
			else 
			{
				char szIP[16] = { 0 };
				IPHost2Str(szIP, m_dwIP);
				LogError("SendMsg fail. error:%d, Dst IP:Port = %s:%d", errno, szIP, m_nPort);
				Terminate();
				return false;
			}
		}
		dwSended += nSend;
		if(dwSended >= dwDataLen)
			return true;
	}
	return true;
}
