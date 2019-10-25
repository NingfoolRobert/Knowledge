
#include "NetService.h"

CNetService::CNetService()
{
	m_pIOMgr = nullptr;
	m_pAcceptIO = nullptr;

}

CNetService::~CNetService()
{

}

bool CNetService::OnInitialUpdate()
{
	if(m_nPort < 1024)
	{
		//TODO 从配置文件中读取服务监听端口号
		
	}
	//初始化配置管理
	m_pIOMgr = new CNetIOMgr;
	if(m_pIOMgr == nullptr)
	{
		LogError("%s(%d) Create Net IO Manager fail.", __FILE__, __LINE__);
		return false;
	}
	//初始化监听端口
	m_pAcceptIO = new CAcceptIO;
	if(m_pAcceptIO == nullptr)
	{
		LogError("%s(%d) Create Accpet IO Manager fail.", __FILE__, __LINE__);
		return false;
	}
	


	if(!m_pAcceptIO->OnInitialUpdate(this))
	{
		return false;
	}

	if(!m_pIOMgr->AddNetIO(m_pAcceptIO))
	{
		return false;
	}
	



	
	
	return true;
}

bool CNetService::OnTimeOut(struct tm* pTime)
{
	return true;
}

bool CNetService::OnSecondIdle()
{
	return true;
}

bool CNetService::OnTerminate()
{
	return true;
}

CNetClient* CNetService::CreateNetClient()
{
	CNetClient* pNetClient = new CNetClient;
	if(nullptr == pNetClient)
		return nullptr;
	
	return pNetClient;
}


