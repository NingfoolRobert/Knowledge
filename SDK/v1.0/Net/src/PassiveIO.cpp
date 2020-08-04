#include "PassiveIO.h"
#include "IOMgr.h"

	
CPassiveIO::CPassiveIO(): m_tLastMsg(0),m_tBreak(0)
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
