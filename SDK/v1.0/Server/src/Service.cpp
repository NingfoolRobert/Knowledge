
#include "Service.h"
#include <signal.h>
#include <string.h>
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono> 

#include "version.h"
#include "marc.h"
#include "versionBase.h"
#include "versionObject.h"


using namespace std;

std::atomic<int> nTimeCount(0);

class CService* g_Service = nullptr;

CService::CService()
{	
	g_Service = this;
	m_tStart = time(nullptr);
}

CService::~CService()
{
//	Terminate();
}

bool CService::Execute(const char* pszCommand)
{
	OnRegisterVersion();
	RegisterVersion("BaseService", BASE_VERSION_VALUE);
	RegisterVersion("ObjectService", OBJECT_VERSION_VALUE);
	//
	if(pszCommand != nullptr && 0 != strlen(pszCommand) )
	{
		if(strncmp(pszCommand, "-V",2) == 0 || strncmp(pszCommand, "--version", 9) ==0)
		{
			printf("Version:\t%s\r\nDate:\t\t%s %s\n", _SVR_VERSION_, __DATE__, __TIME__);
		}
		else 
		{
			printf("\r\n-help\n	-V/--version: Output the service version No.\n");
		}
		return 0;
	}
	//
	Singleton<CLogFile>::GetInstance().WriteData("\n\n", strlen("\n\n"));
	LogInfo("Init Service ...");
	
	if(!OnInitialUpdate())
	{
		LogError("Init Service fail....");
		exit(-1);
		return false;
	}
	
	LogInfo("%s Start Success...", m_szServiceName);
	m_bStop = false;	
	
	auto now = std::chrono::system_clock::now();
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsLock);
		//std::cv_status nStatus = m_condStop.wait_for(locker, std::chrono::seconds(1));
		auto texpire = now + std::chrono::milliseconds(1000);
		if(m_condStop.wait_until(locker, texpire, [&]()->bool{return m_bStop;}))
		//if(nStatus != std::cv_status::timeout)
		{
			InvokeTerminate();
			break;
		}
	
		time_t tNow = time(nullptr);
		struct tm pTime;// = nullptr;
		localtime_r(&tNow, &pTime);
		OnSecondIdle();
		if(nTimeCount == 60)
		{
			OnTimeOut(&pTime);
			nTimeCount = 0;
		}
		nTimeCount++;
		now = texpire;
	}

	OnTerminate();
	LogInfo("Stop Service ....\r\n");
	return true;
}

//
void CService::Terminate()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	m_condStop.notify_all();
	m_bStop = true;
}

bool CService::SysRun()
{
	return true;		
}
	
void CService::OnRegisterVersion()
{
	strcpy(m_szVersion, "0.0.0.0");
}
	
void CService::RegisterVersion(const char* pszVersionName, const char* pszVersionValue)
{
	if(nullptr == pszVersionName || nullptr == pszVersionValue)
		return ;
	
	VERSIONINFO stInfo;
	strcpy(stInfo.szVersionName, pszVersionName);
	strcpy(stInfo.szVersionValue, pszVersionValue);

	m_listVersion.emplace_back(stInfo);
}
