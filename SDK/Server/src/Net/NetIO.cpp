#include "NetIO.h"
	
CNetIO::CNetIO():m_nEventType(0), m_nNewEventType(0)
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
