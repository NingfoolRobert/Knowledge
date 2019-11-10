
#include "AgentService.h"
#include "Log.h"

class CBufferMgr*	g_pBufferMgr = nullptr;
class CAgentService* g_ciccAgentService = nullptr ;

CAgentService::CAgentService()
{
	g_ciccAgentService = this;
}

CAgentService::~CAgentService()
{
	if(m_pPubLog != nullptr)
	{
		delete m_pPubLog;
		m_pPubLog = nullptr;
	}
}

bool CAgentService::OnInitialUpdate()
{
	if(!CNetObjService::OnInitialUpdate())
	{
		return false;
	}

	//
	m_pPubLog =  new CZMQPub<CAgentService>;
	if(nullptr == m_pPubLog)
	{
		LogError("%s(%d) alloc ZMQPUB fail.", __FILE__, __LINE__);
		return false;
	}
	
	return true;
}

bool CAgentService::OnTimeOut(struct tm* pTime)
{
	
	return true;
}

bool CAgentService::OnSecondIdle()
{
	return true;
}


bool CAgentService::OnTerminate()
{
	return true;
}


bool CAgentService::PostMsg(const char* pszTopic, CBuffer* pBufMsg)
{
	if(nullptr == m_pPubLog)
	{
		LogError(" %s(%d) PubLog is nullptr.", __FILE__, __LINE__);
		return false;
	}

	m_pPubLog->PostMsg(pszTopic, pBufMsg);

	ReleaseBuffer(pBufMsg);

	return true;
}


CBuffer*  CAgentService::GetBuffer(int nLength, const char* pszFileName = "", int nLine =  0)
{
	return g_pBufferMgr->GetBuffer(nLength, pszFileName, nLine);
}
	

bool CAgentService::ReclaimBuffer(CBuffer* pBuffer)
{
	g_pBufferMgr->ReleaseBuffer(pBuffer);
	return true;
}

//线程安全 获取Pub源消息序列号 
unsigned long  CAgentService::GetPubSerialNum(int nIncrement/* = 1*/)
{
	if(nullptr == m_pPubLog)
	{
		return 0;
	}
	//
	m_pPubLog->m_ulSerialNum += nIncrement;
	return  m_pPubLog->m_ulSerialNum;
}


bool CAgentService::SendLogMsg(const std::string strDirName, std::vector<std::string>& listLogItem)
{
	if(0 == strDirName.length() || 0 == listLogItem.size())
	{
		return false;
	}

	auto pLogDir = m_pLogMgr->FindLogDir(strDirName);
	if(pLogDir == nullptr)
	{
		return false;
	}
	
	char szTopic[256] = { 0 };
	CBuffer stBuf;
	
	pLogDir->SerialMsg(szTopic, &stBuf, listLogItem);

	PostMsg(szTopic, &stBuf);

	return true;
}

CBuffer*  CAgentService::GetBuffer(int nLength, const char* pszFileName/* = ""*/, int nLine/* =  0*/)
{
	if(g_pBufferMgr == nullptr)
		return nullptr;
	return g_pBufferMgr->GetBuffer(nLength, pszFileName, nLine);
}
	
bool CAgentService::ReclaimBuffer(CBuffer* pBuffer)
{
	g_pBufferMgr->ReleaseBuffer(pBuffer);
	return true;
}
