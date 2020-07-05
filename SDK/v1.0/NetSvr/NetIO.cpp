#include "NetIO.h"
	
CNetIO::CNetIO():m_nEventType(0), m_nNewEventType(0), m_nRef(1)
{
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
	return false;
}
	
bool CNetIO::OnTerminate()
{
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
	if(!IsReadable())
		m_nNewEventType |= EPOLLIN;
}

void CNetIO::PermitWrite()
{
	if(!IsWriteable())
		m_nNewEventType |= EPOLLOUT;
}

void CNetIO::ForbidRead()
{
	m_nNewEventType &= ~EPOLLIN;
}

void CNetIO::ForbidWrite()
{
	m_nNewEventType &= ~EPOLLOUT;

}
	
bool CNetIO::IsReadable()
{
	return m_nEventType & EPOLLIN;
}
bool CNetIO::IsWriteable()
{
	return m_nEventType & EPOLLOUT;
}
