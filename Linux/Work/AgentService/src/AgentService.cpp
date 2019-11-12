
#include "AgentService.h"
#include "Log.h"
#include "tinyxml2.h"


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

	m_pSThreadPool  = new CSThreadPool;
	if(m_pSThreadPool == nullptr)
	{
		LogError("%s(%d) alloc SThreadPool fail.", __FILE__, __LINE__);
		return false;
	}
	//
	m_pSThreadPool->Init(2,10);
	//
	m_pPubLog =  new CZMQPub();
	if(nullptr == m_pPubLog)
	{
		LogError("%s(%d) alloc ZMQPUB fail.", __FILE__, __LINE__);
		return false;
	}


	char szURI[32] = { 0 };
	
	//GetXMLAttributeString("NetSetting", "URI", "", szURI);
	
	using namespace tinyxml2;
	try 
	{
		XMLDocument doc;
		if(doc.LoadFile(m_szConfigureFileName))
		{
			return false;
		}
		XMLElement* pRoot = doc.FirstChildElement("Service");
		if(pRoot == nullptr)
			return false;

		XMLElement* pNet = pRoot->FirstChildElement("NetSetting");
		if(pNet == nullptr)
			return false;

		XMLElement* pPub = pNet->FirstChildElement("Pub");
		if(pPub == nullptr)
			return false;
		auto pURI = pPub->Attribute("URI");
		if(pURI == nullptr)
			return false;
		strcpy(szURI, pURI);

	}
	catch(std::exception e)
	{
		LogError("%s(%d) Parse XML File fail.", __FILE__, __LINE__);
		return false;
	}

//	if(!m_pPubLog->Init(szURI))
//	{
//		LogError("%s(%d) Pub Initial fail.", __FILE__, __LINE__);
//		return false;
//	}

	//LogMgr Init 
	m_pLogMgr = std::make_shared<CLogMgr>();
	if(nullptr == m_pLogMgr)
	{
		LogError("%s(%d) alloc CLogMgr fail.", __FILE__, __LINE__);
		return false;
	}
	if(!m_pLogMgr->Init())
	{
		LogError("%s(%d) LogMgr Init fail.", __FILE__, __LINE__);
		return false;	
	}

	return true;
}

bool CAgentService::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}

	if(m_pLogMgr)
	{
		m_pLogMgr->TimeOut(pTime);		//分钟定时
	}
	return true;
}

bool CAgentService::OnSecondIdle()
{
	if(m_pLogMgr)
	{
		m_pLogMgr->SecondIdle();		//秒级定时
	}
	return true;
}

bool CAgentService::InvokeTerminate()
{
	return true;
}

bool CAgentService::OnTerminate()
{
	m_pSThreadPool->Terminate();
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

	return true;
}

bool CAgentService::PostTask(Task& task)
{
	m_pSThreadPool->AddTask(task);
	return true;
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
	return  m_pPubLog->GetPubSerialNum(nIncrement);
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
	
	pLogDir->SerialMessage(szTopic, &stBuf, listLogItem);

	PostMsg(szTopic, &stBuf);

	return true;
}

CBuffer*  CAgentService::GetBuffer(int nLength, const char* pszFileName/* = ""*/, int nLine/* =  0*/)
{
	if(g_pBufferMgr == nullptr)
		return nullptr;
	return g_pBufferMgr->GetBuffer(nLength, pszFileName, nLine);
}
	
bool CAgentService::GetXMLAttributeString(const char* pszSection, const char* pszKey, const char* pszDefaultValue, char* pszValue, const char* pszFileName/* = nullptr*/)
{
	if(pszSection == nullptr || pszKey == nullptr || pszValue == nullptr)
		return false;

	using namespace tinyxml2;
	
	try 
	{
		XMLDocument doc;
		XMLError error;
		if(pszFileName == nullptr)
			error = doc.LoadFile(m_szConfigureFileName);
		else 
			error = doc.LoadFile(pszFileName);
		if(error != 0)
		{
			strcpy(pszValue, pszDefaultValue);
			LogError("%s(%d) Load XML File fail. FileName: %d", __FILE__, __LINE__, m_szConfigureFileName);
			return false;
		}
		
		XMLElement* pRoot = doc.FirstChildElement("Service");
		if(pRoot == nullptr)
		{
			strcpy(pszValue, pszDefaultValue);
			return true;
		}

		XMLElement* pSec = pRoot->FirstChildElement(pszSection);
		if(pSec == nullptr)
		{
			strcpy(pszValue, pszDefaultValue);	
			return false;
		}
		auto pValue = pSec->Attribute(pszKey);
		if(pValue == nullptr)
			strcpy(pszValue, pszDefaultValue);
		else 
			strcpy(pszValue, pValue);
	}
	catch(std::exception e)
	{
		LogError("%s(%d) LoadXMLFile fail.", m_szConfigureFileName);
		return false;
	}

	return true;
}
	
