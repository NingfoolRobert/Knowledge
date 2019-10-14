#include "NotifyMgr.h"

CNotifyMgr::CNotifyMgr()
{
	m_pContactor = nullptr;
}

CNotifyMgr::~CNotifyMgr()
{
	if(m_pContactor)
	{
		delete m_pContactor;
		m_pContactor = nullptr;
	}

}

bool CNotifyMgr::Init(const char* pszConfigPath)
{
	
	return true;
}
	
bool CNotifyMgr::UpdateConfig(const char* pszConfigFileName)
{

	return true;
}
bool CNotifyMgr::Send(const char* pszWarnInfo)
{
	return true;
}

bool CNotifyMgr::Add(CNotify* pNotify)
{
	return true;
}

bool CNotifyMgr::Del(CNotify* pNotify)
{
	return true;
}
