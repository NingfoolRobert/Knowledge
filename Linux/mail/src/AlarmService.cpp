
#include "AlarmService.h"
#include <signal.h>
#include <sys/types.h>

//static void WarnningThread(CAlarmService* pService)
//{
//	if(pService == nullptr)
//	{	
//		LogWarn("%s(%s)pService == NULL", __FILE__, __FUNCTION__);
//		return ;	
//	}	
//	
//	pService->SendNotify();
//}


class CAlarmService* g_ciccAlarmService = nullptr;

CAlarmService::CAlarmService()
{
	m_pNotify = nullptr;
	g_ciccAlarmService = this;
}

CAlarmService::~CAlarmService()
{

}

bool CAlarmService::OnInitialUpdate()
{	
	signal(SIGPIPE,SIG_IGN);

	//处理对象注册
	REGISTER_CLASS(BaseHandler,CAppUser);
	///
	if(!CNetObjService::OnInitialUpdate())
		return true;

	m_pNotify = new CNotifyMgr;
	if(nullptr == m_pNotify)
	{
		LogError("%s(%d) malloc CNotifyMgr fail.", __FILE__, __LINE__);
		return false;
	}
	if(!m_pNotify->Init("./AlarmService.xml"))
	{
		return false;
	}

	InitWarnningLevel();

//	SendWarningInfo(1, "FIX", "Fix业务");
	return true;
}

bool CAlarmService::OnTimeout(struct tm* pTime)
{
	if(!CNetObjService::OnTimeOut(pTime))
		return false;
		
	m_pNotify->OnTimeout(pTime);	
	LogInfo("OnTimeOut...");

	return true;
}

bool CAlarmService::OnSecondIdle()
{
	if(!CNetObjService::OnSecondIdle())
		return false;

	return true;
}

bool CAlarmService::InvokeTerminate()
{
	LogInfo("InvokeTerminate...");
	return true;
}

bool CAlarmService::OnTerminate()
{
	if(m_pNotify)
	{
		delete m_pNotify;
		m_pNotify = nullptr;
	}
	LogInfo("Service Stop...");
	return true;
}

BaseHandler* CAlarmService::CreateHandler(int nType)
{
	switch(nType)
	{
		case CICC_SERVICE_TYPE_MONITOR:
			return CREATE_CLASS(BaseHandler, CAppUser);
		default:
			break;
	}
	return nullptr;
}

bool CAlarmService::SendWarningInfo(const int nLevel, const std::string  strAppType, CBuffer* pBuffer)
{
	if(nullptr == pBuffer)
	{
		LogWarn("Warnning Infomation is null.");
		return false;
	}
	if(nullptr == m_pNotify)
	{
		LogError("%s(%d) [E] Notify Manager is nullptr. Info:%s", __FILE__, __LINE__, pBuffer->GetBufPtr());
		return false;
	}
	if(!m_pNotify->Send(nLevel, strAppType, pBuffer))
	{
		LogWarn("Send Warnning info fail.Warnning: %s", pBuffer->GetBufPtr());
		return false;
	}
	return true;
}

int CAlarmService::GetWarningLevel(std::string pszWarnLevel)
{
	auto it = m_mapWarningLevel.find(pszWarnLevel);
	if(it != m_mapWarningLevel.end())
		return it->second;

	return 1;
}

void CAlarmService::InitWarnningLevel()
{
	m_mapWarningLevel.clear();
	
	m_mapWarningLevel["TEST"]			= 1;	
	m_mapWarningLevel["NOTICE"]			= 2;
	m_mapWarningLevel["WARNING"]		= 3;
	m_mapWarningLevel["ERROR"]			= 4;
	m_mapWarningLevel["FATAL"]			= 5;

	LogInfo("WarnningLevel Count: %d", m_mapWarningLevel.size());
#ifdef _DEBUG
	CBuffer stBuf;
	char szTmp[20] = { 0 };
	for(auto it = m_mapWarningLevel.begin(); it != m_mapWarningLevel.end(); ++it)
	{
		sprintf(szTmp,"%s:%d ", it->first.c_str(), it->second);

		if(!stBuf.Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}
#endif 
}
