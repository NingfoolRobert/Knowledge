#include "LogDir.h"
#include "AgentService.h"
#include "PBDefineMessage.h"


using namespace Protocol::Protobuf;

CLogDir::CLogDir()
{

}

CLogDir:: ~CLogDir()
{

}

bool CLogDir::Init(PLOGDIRINFO pLogDirInfo, std::set<std::string>& listIgnoreLog)
{
	return true;
}

bool CLogDir::Init(const char* pszDirName, const char* pszAppType, const char* pszBusiss, int nID, int nTTL/* = 360*/)
{
	return true;
}

bool CLogDir::GetCollectLog(std::vector<LOGPtr>& listLog, std::vector<LOGPtr>& listIgnoreLog)
{
	return true;
}

void CLogDir::CheckLogValid()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	for(auto it = m_listLogInfo.begin(); it != m_listLogInfo.end(); )
	{
		auto pLog = it->second;
		if(!pLog->m_bCheck)
			m_listLogInfo.erase(it++);
	}
}

void CLogDir::GetMsgTopic(char* pszTopic)
{
	
}

bool CLogDir::AddLogObj(const char* pszLogFileName)
{
	return true;
}

bool CLogDir::DelLogObj(const char* pszLogFileName)
{
	return true;
}


void CLogDir::CollectLogItem()
{
	std::vector<std::string> TmpLogItem;
	char szTopic[256] = { 0 };
	CBuffer stBuf;
	for(size_t i = 0; i < m_listLogInfo.size(); ++i)
	{
		auto pLog = m_listLogInfo[i];
		pLog->GetLogItem(TmpLogItem);
		
		//
		memset(szTopic, 0, sizeof(szTopic));
		if(!SerialMessage(szTopic, &stBuf, TmpLogItem))
			continue;
		
		g_ciccAgentService->PostMsg(szTopic, &stBuf);
	}
}

void CLogDir::FormatTopic(char* pszTopic)
{
	sprintf(pszTopic, "C^L^%s^%s^%d^", m_szType, m_szService, m_nID);
}

bool CLogDir::SerialMessage(char* pszTopic, CBuffer* pBuf, std::vector<std::string>& listLogItem)
{
	if(pszTopic == nullptr ||  pBuf == nullptr)
	{
		return false;
	}
	
	int nCount = 0;
	FormatTopic(pszTopic);

	pBuf->Clear();

	CICCDataFeed data;

	auto pRawLog = data.mutable_rawlogarray();
	if(pRawLog == nullptr)
	{
		return false;
	}
	
	
//TODO 序列化消息并追加到Buffer中 
	
	
	

	return true;
}

void CLogDir::CheckLogList()
{
	for_each(m_listLogInfo.begin(), m_listLogInfo.end(), [&](std::map<std::string, LOGPtr>::value_type& Log){
			Log.second->m_bCheck = false;
			});



	std::vector<std::string> listFile;
	CommonHelper::GetAllFileName(m_szLogDir, listFile, true);
	
	for_each(listFile.begin(), listFile.end(), [&](const std::string strFileName){
			//TODO 新增文件
			auto it = m_listAllFileName.find(strFileName);
			if(it == m_listAllFileName.end())
			{
				//TODO 
				auto pLog = std::make_shared<CLogCollect>();
				if(pLog == nullptr)
				{
					return ;
				}
				
				pLog->Init(strFileName.c_str());
				m_listLogInfo[strFileName] = pLog;
			}
			});
	

	//TODO 更新文件信息 
	
	for_each(m_listLogInfo.begin(), m_listLogInfo.end(), std::bind(&LogDir::UpdateLogInfo, this, std::placeholders::_1));

	//TODO 采集日志 
	
}


void CLogDir::UpdateLogInfo(std::map<std::string, LOGPtr>::value_type & value)
{
	long lFileSize  = 0;
	time_t tModify = 0;
	if(!CommonHelper::GetFileInfo(value.first.c_str(), lFileSize, tModify))
	{
		value.second->m_bCheck = false;
		m_listIgnoreLog.insert(value.first);
		return ;
	}
	
	auto pLog = value.second;
	if(nullptr != pLog)
		pLog->UpdateFileInfo(lFileSize, tModify);
}


void CLogDir::LoadXMLDirInfo(tinyxml2::XMLElement* pXMLDir)
{
	using namespace tinyxml2;
	if(nullptr == pXMLDir)
	{
		return ;
	}
	//TODO 获取正常文件读取 
	//
	//
	//
	//TODO 忽略文件
}
