#include "NetIO.h"
	
CNetIO::CNetIO():m_pOwner(nullptr),m_nEventType(0), m_nNewEventType(0)
{
	m_nRef = 1;
}
CNetIO::~CNetIO()
{
}

bool CNetIO::OnRecv()
{
	return false;
}

bool CNetIO::OnSend() 
{
	return false;
}


bool CNetIO::OnClose()
{
	if(m_pOwner)
		m_pOwner->OnClose();
	m_nNewEventType = EPOLL_EVENT_TYPE_CLOSE;
	return false;
}
	
	
int	CNetIO::AddRef()
{
	return ++m_nRef;
}

void CNetIO::Release()
{
	--m_nRef;
	if(m_nRef <= 0)
		delete this;
}
	
void CNetIO::PermitRead()
{
	m_nNewEventType |= EPOLLIN;
}

void CNetIO::PermitWrite()
{
	m_nNewEventType |= EPOLLOUT;
}

void CNetIO::ForbidRead()
{
	m_nNewEventType = m_nEventType & ~EPOLLIN;
}

void CNetIO::ForbidWrite()
{
	m_nNewEventType = m_nEventType & ~EPOLLOUT;
}
	
bool CNetIO::IsReadable()
{
	return m_nEventType & EPOLLIN;
}

bool CNetIO::IsWriteable()
{
	return m_nEventType & EPOLLOUT;
}
	
void CNetIO::ProcessMsg()
{
	
}
	
void CNetIO::SetOwner(CNetIO* pOwner)
{
	m_pOwner = pOwner;
}
	
bool CNetIO::OnMsg(PHEADER& pHeader)			//消息到达接口
{
	if(m_pOwner)
	{
		m_pOwner->OnMsg(pHeader);
		m_pOwner->Release();
	}

	return true;
}
	
void CNetIO::OnTickCnt()							//定时调用
{
	if(m_pOwner)
	{
		m_pOwner->OnTickCnt();
	}
}
