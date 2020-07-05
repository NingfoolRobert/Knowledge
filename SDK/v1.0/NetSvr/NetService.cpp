#include "NetService.h"

	
class CNetService* g_pNetService = nullptr;
CNetService::CNetService():m_bStop(false)
{
	g_pNetService = this;

}

CNetService::~CNetService()
{
	
}
	
bool CNetService::OnNetConnect(CNetClient* pNet)
{
	if(nullptr == pNet)
		return false;

	//NoDey 
	//pNet->SetSockOpt()	;
	//Send Timeout ;
	//Recv TimeOut;

	return true;
}
	
bool CNetService::OnNetMsg(CNetClient* pNetClient, PHEADER pHeader)
{
	if(nullptr == pNetClient || nullptr == pHeader)
		return false; 

	pNetClient->AddRef();
	std::unique_lock<std::mutex> locker(m_clsWork);
	m_listWorkClient.Add(pNetClient);
	m_condWork.notify_one();
	return true;
}
	
bool CNetService::OnNetBreak(CNetClient* pNetClient)					//网络断开
{
	if(nullptr == pNetClient)
		return false;
	//
	CUserObject* pUserObject = pNetClient->GetUserObject();
	if(pUserObject)
	{
		pUserObject->OnBreak();
		pUserObject->m_tBreak = time(NULL);
		pUserObject->Release();
	}
	//TODO 
	auto it = m_mapNetIO.find(pNetClient->Detach());
	if(it != m_mapNetIO.end())
	{
		return false;
	}
	m_mapNetIO.erase(it);
	pNetClient->Release();

	//
	return true;
}

bool CNetService::OnNetTickCnt(CNetClient* pNetClient)				//网络定时调用
{
	if(nullptr == pNetClient)
		return false;
	
	return true;
}


void CNetService::ActiveEpollThread()
{
	while(!m_bStop)
	{


//		int nCnt = epoll_wait(, 1000);
	}
		
}

void CNetService::ActiveWorkThread()
{
	CNetClient* pNetClient = nullptr;
	while(!m_bStop)
	{
		std::unique_lock<std::mutex> locker(m_clsWork);
		m_condWork.wait(locker, [&]()->bool{ return !m_listWorkClient.empty(); });
		locker.unlock();
		while(!m_listWorkClient.empty())
		{
			m_clsWork.lock();
			pNetClient = m_listWorkClient.Get();
			m_clsWork.unlock();
			
			if(nullptr == pNetClient)
				continue;
			
			pNetClient->ProcessMsg();
			pNetClient->Release();
		}
	}
}

