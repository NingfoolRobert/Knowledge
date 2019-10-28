
#include "NetService.h"
#include "AcceptIO.h"
#include "Log.h"
#include "IOMgr.h"

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
	//初始化配置管理
	m_pIOMgr = new CIOMgr;
	if(m_pIOMgr == nullptr)
	{
		LogError("%s(%d) Create Net IO Manager fail.", __FILE__, __LINE__);
		return false;
	}

	// IO Manager 初始化
	if(m_pIOMgr->OnInitialUpdate(this, m_pAcceptIO))
	{
		LogError("%s(%d) NetIO Manager Init fail.", __FILE__, __LINE__);
		return false;
	}

	LogInfo("Net Service init Success. Port: %d", m_nPort);	
	return true;
}

bool CNetService::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
		return false;
	if(m_pIOMgr)
	{
		m_pIOMgr->OnTimeOut(pTime);
	}
	//
	
	return true;
}

bool CNetService::OnSecondIdle()
{
	m_pIOMgr->OnSecondIdle();

	return true;
}

bool CNetService::OnTerminate()
{
	m_pAcceptIO->Close();
	delete m_pAcceptIO;


	m_pIOMgr->OnTerminate();

	delete m_pIOMgr;

	return true;
}

CNetClient* CNetService::CreateNetClient()
{
	CNetClient* pNetClient = new CNetClient;
	if(nullptr == pNetClient)
		return nullptr;
	
	return pNetClient;
}

bool CNetService::OnNetConnect(CNetClient* pNetClient)
{
	return true;
}

bool CNetService::OnNetMsg(CNetClient* pNetClient, PHEADER pMsg)
{
	if(nullptr == pNetClient || nullptr == pMsg)
		return false;
	return true;
}

bool CNetService::OnNetBreak(CNetClient* pNetClient)
{
	return true;
}

bool CNetService::OnNetTickCount(CNetClient* pNetClient)
{
	return true;
}

bool CNetService::AddNetClient(CNetClient* pNetClient)
{
	if(nullptr == m_pIOMgr)
	{
		return false;
	}
	
	m_pIOMgr->AddNetIO(pNetClient);
	
	return true;
}
