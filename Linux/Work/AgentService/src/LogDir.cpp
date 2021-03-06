#include "LogDir.h"
#include "AgentService.h"
#include "CommonHelper.h"
#include "Log.h"
#include "PBMessageDefine.h"


using namespace Protocol::Protobuf;

CLogDir::CLogDir()
{
	m_bValid = true;
}

CLogDir:: ~CLogDir()
{

}

bool CLogDir::Init(PLOGDIRINFO pLogDirInfo, std::shared_ptr<LASTREADINFO> pReadDirInfo/* = nullptr*/)
{
	if(nullptr == pLogDirInfo)
	{
		return false;
	}

	m_nTTL = pLogDirInfo->nTTL;
	strcpy(m_szDirName, pLogDirInfo->szDirName);
	strcpy(m_szService, pLogDirInfo->szServiceType );
	strcpy(m_szType,pLogDirInfo->szType);
	m_nID = pLogDirInfo->nID;


	std::vector<std::string> listAllFile; 
	CommonHelper::GetAllFileName(m_szDirName, listAllFile, true);
	//
	std::unique_lock<std::mutex>  locker(m_clsLock);
	//读取已存在的文件信息
	std::map<std::string, LOGINFO> mapLogInfo;
	if(pReadDirInfo != nullptr)
	{
		for_each(pReadDirInfo->listReadInfo.begin(), pReadDirInfo->listReadInfo.end(), [&](const LOGINFO& stInfo){
 				mapLogInfo.insert(make_pair(stInfo.szFileName,stInfo));
				});
		//忽略文件
		for(auto it = pReadDirInfo->listIgnore.begin(); it != pReadDirInfo->listIgnore.end(); ++it)

			m_listIgnoreLog.insert(*it);
	}
	//
	
	for(size_t i = 0; i < listAllFile.size(); ++i)
	{
		if(!CommonHelper::IsTxtFile(listAllFile[i].c_str()))
			continue;
		auto pLog = std::make_shared<CLogCollect>();
		if(pLog == nullptr)
		{
			LogError("%s(%d) Alloc CLogCollect fail.", __FILE__, __LINE__);
			continue;
		}

		if(m_listIgnoreLog.count(listAllFile[i]))
		{
			//标记为忽略文件
			continue;
		}
		

		auto it = mapLogInfo.find(listAllFile[i]);

		if(it == mapLogInfo.end())
			pLog->Init(listAllFile[i].c_str(), m_szDirName, nullptr);
		else 
			pLog->Init(listAllFile[i].c_str(),m_szDirName, &it->second);

		m_listLogInfo.insert(std::make_pair(listAllFile[i], pLog));
	}
	

	return true;
}

void CLogDir::CheckLogValid()
{
	std::unique_lock<std::mutex> locker(m_clsLock);
	for(auto it = m_listLogInfo.begin(); it != m_listLogInfo.end(); )
	{
		LOGPtr pLog = it->second;
		//不存在文件 直接从内存中删除
		if(!pLog->m_bCheck)
		{
			m_listLogInfo.erase(it++);
			continue;
		}
		//过期文件从内存中清理
		if(pLog->m_bTTL)
		{	
			std::string str = it->first;
			m_listIgnoreLog.insert(str);	//	保存进入忽略文件
			m_listLogInfo.erase(it++);
			
			continue;
		}
		++it;
	}
}


bool CLogDir::SerialMessage(char* pszTopic, CBuffer* pBuf, std::vector<std::string>& listLogItem)
{
	if(pszTopic == nullptr ||  pBuf == nullptr || 0 == listLogItem.size())
	{
		return false;
	}
	//
	unsigned long  ulSerialNum = g_ciccAgentService->GetPubSerialNum();
	sprintf(pszTopic, "C^L^%s^%s^%d^%ld", m_szType, m_szService, m_nID, ulSerialNum);

	pBuf->Clear();

	CICCDataFeed data;

	auto pRawLog = data.mutable_rawlogmsgdata();
	if(pRawLog == nullptr)
	{
		return false;
	}
	
	
// 序列化消息并追加到Buffer中 
//
	char szID[16] = { 0 };
	sprintf(szID, "%d", m_nID);
	pRawLog->set_serviceid(szID);
	pRawLog->set_servicetype(m_szService);
	pRawLog->set_infotype(m_szType);

	time_t tNow = time(NULL);
	
	pRawLog->set_localtime(CommonHelper::TransferLocalTimeInt2Int64(tNow));
	
	pRawLog->set_serialnum(ulSerialNum);
	
	for(size_t i = 0; i < listLogItem.size(); ++i)
	{
		auto pRawEntry = pRawLog->add_rawloglist();
		if(nullptr == pRawEntry)
		{
			LogError("%s(%d) Protobuf Alloc RawLog fail.LogItem: %s", __FILE__, __LINE__, listLogItem[i].c_str());
			continue;
		}
		//RawLogEntry
		{
			//pRawLog->set_level("");
			pRawEntry->set_logentry(listLogItem[i].c_str());
		}
		
	}

#ifdef DEBUG 
	LogInfo("PB LogItem Count: %d", listLogItem.size());
#endif

	std::vector<char> Buf;
	Buf.resize(data.ByteSize());
	
	if(!data.SerializeToArray(&Buf[0], data.ByteSize()))
	{		
		LogError("%s(%d) SerializeToArray fail.", __FILE__, __LINE__);
		return false;
	}
	
	if(!pBuf->Append(&Buf[0], data.ByteSize()))
	{
		LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void CLogDir::CheckLogList()
{
	std::unique_lock<std::mutex> locker(m_clsLock);

	for(auto it = m_listLogInfo.begin(); it != m_listLogInfo.end(); ++it)
	{
		LOGPtr pLog = it->second;
		pLog->m_bCheck =  false;

	}
	//更新文件夹信息 

	std::vector<std::string> listFile;
	CommonHelper::GetAllFileName(m_szDirName, listFile, true);
	
	for(size_t i = 0; i < listFile.size(); ++i)
	{
		std::string strFileName = listFile[i];
		if(!CommonHelper::IsTxtFile(listFile[i].c_str()))
			continue;
		auto it = m_listAllFileName.find(strFileName);
		if(it == m_listAllFileName.end())
		{
 			//TODO 
			LOGPtr pLog = std::make_shared<CLogCollect>();
			if(pLog == nullptr)
			{
				return ;
			}
			
			pLog->Init(strFileName.c_str(), m_szDirName);
			m_listLogInfo.insert(make_pair(strFileName, pLog)); 
	 		m_listAllFileName.insert(strFileName);
		}
		else 
		{
			auto pIt = m_listLogInfo.find(strFileName);
			if(pIt != m_listLogInfo.end())
	 			pIt->second->m_bCheck = true;
			else 
			{
			//	lLOGPtr pLog = std::make_shared<CLogCollect>();
			//	pLog->Init(strFileName.c_str(), m_szDirName);
			//	m_listLogInfo.insert(make_pair(strFileName, pLog));
			}
		}

	}

	//TODO 更新文件信息 
	
	for_each(m_listLogInfo.begin(), m_listLogInfo.end(), std::bind(&CLogDir::UpdateLogInfo, this, std::placeholders::_1));	
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
		pLog->UpdateFileInfo(tModify, lFileSize,  m_nTTL);
}



void CLogDir::GetDirInfo(std::shared_ptr<LASTREADINFO> pLogDirInfo)
{
	if(nullptr == pLogDirInfo)
		return ;
	
	for(auto it = m_listIgnoreLog.begin(); it != m_listIgnoreLog.end(); ++it)
	{
		pLogDirInfo->listIgnore.insert(*it);
	}

	//
	for_each(m_listLogInfo.begin(), m_listLogInfo.end(), [&](const std::map<std::string,std::shared_ptr<CLogCollect> >::value_type& value)
			{
				auto pLog = value.second;
				LOGINFO stInfo;
				pLog->GetLogInfo(&stInfo);
				pLogDirInfo->listReadInfo.push_back(stInfo);
			});

}

void CLogDir::GetLastestLogItem()
{
	//更新文件夹信息
	LogInfo("Check before: %d", m_listLogInfo.size());
	CheckLogList();
	//更新文件信息 
//	UpdateFileInfo();
	//获取最新条目信息	
	LogInfo("Check After: %d", m_listLogInfo.size());
	//清除过期以及不存在文件
	CheckLogValid();
	LogInfo("CheckLogValid after: %d",m_listLogInfo.size());
	//

	for(auto it = m_listLogInfo.begin(); it != m_listLogInfo.end(); ++it)
	{
		auto pLog = it->second;

		pLog->CollectItem();
//		CollectoLogItem(it->first.c_str());
//		std::string str = it->first;
//		g_ciccAgentService->PostTask(&CLogDir::CollectoLogItem, this, str.c_str());
//		std::thread tr1(&CLogDir::CollectLogItem,this, std::ref(pLog)));
//		tr1.detach();
//			g_ciccAgentService->PostTask(&CLogCollect::GetAugementerLogItem, pLog.get());
//				g_ciccAgentService->PostTask(CollectLogItem, pLog);
	}
	//	for_each(m_listLogInfo.begin(),  m_listLogInfo.end(), [&](std::map<std::string, LOGPtr>::value_type & value){
//			auto pLog = value.second; 
//			if(pLog == nullptr) return;
//	//		pLog->GetAugementerLogItem();
//			g_ciccAgentService->PostTask(&CLogCollect::GetAugementerLogItem, pLog.get());
//			});
	LogInfo("Read Dir: %s, File Count: %d", m_szDirName, m_listLogInfo.size());
	
}

void CLogDir::CollectoLogItem(const char* pszLogFileName)
{
	if(pszLogFileName == nullptr )
		return ;
	
	auto it = m_listLogInfo.find(pszLogFileName);
	if(it != m_listLogInfo.end())
	{
		auto pLog = it->second;
		pLog->GetAugementerLogItem();
	}
}
