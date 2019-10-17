
#include "AlarmService.h"

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
	//处理对象注册
	REGISTER_CLASS(BaseHandler,CAppUser);
	///
	if(!CNetObjService::OnInitialUpdate())
		return true;
	LogInfo("Init Service...");

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
	

	
	
	return true;
}

bool CAlarmService::OnTimeout(struct tm* pTime)
{
		
	LogInfo("OnTimeOut...");

	return true;
}

bool CAlarmService::OnSecondIdle()
{
	
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
		case 10:
			return CREATE_CLASS(BaseHandler, CAppUser);
		default:
			break;
	}
	return nullptr;
}

