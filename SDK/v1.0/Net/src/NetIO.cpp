#include "NetIO.h"
#include "LogFile.h"
	
CNetIO::CNetIO():m_pOwner(nullptr),m_nEventType(0), m_nNewEventType(0)
{
	m_nRef = 0;
}
CNetIO::~CNetIO()
{
	if(m_pOwner) 
		m_pOwner->SetOwner(NULL);
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
	
	
int	CNetIO::AddRef(const char* pszFunciton)
{
#if _TEST
	LogTrace("%s, %08X,  ADD %d", pszFunciton, this, m_nRef.load() + 1);	
#endif 
	return ++m_nRef;
}

void CNetIO::Release(const char* pszFunciton)
{
	if(--m_nRef <= 0)
	{
#if _TEST
		LogTrace("%s, %08X, DEL %d", pszFunciton, this, m_nRef.load());	
#endif 
		delete this;
		return ;
	}
#if _TEST 
	LogTrace("%s %08X, RELEASE %d", pszFunciton, this, m_nRef.load());	
#endif
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
		m_pOwner->AddRef();
		m_pOwner->OnTickCnt();
		m_pOwner->Release();
	}
}
