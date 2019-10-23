
#pragma once 



#include "NotifyMgr.h"
#include "NetObjService.h"
#include "AppUser.h"

#include <queue>

typedef struct stWarnInfo
{
	int nLevel;
	std::string strAppType;
	std::string strWarnningInfo;
}WARNINFO,*PWARNINFO;


class CAlarmService : public CNetObjService 
{
public:
	CAlarmService();
	virtual ~CAlarmService();

public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeout(struct tm* pTime);

	virtual bool OnSecondIdle();
	
	virtual bool InvokeTerminate();
	
	virtual bool OnTerminate();

	virtual BaseHandler* CreateHandler(int nType);
	
public:
	
	bool SendWarningInfo(const int nLevel, const std::string  strAppType, CBuffer* pBuffer);

	int GetWarningLevel(std::string pszWarnLevel);

	void SendNotify();

	void Test();
	
protected:
	
	void  InitWarnningLevel();
private:
	CNotifyMgr*		m_pNotify;	//通知管理类

	std::queue<PWARNINFO>  m_listWarnInfo;
	
	std::map<std::string, int>	m_mapWarningLevel;
};

extern class CAlarmService*	g_ciccAlarmService;
