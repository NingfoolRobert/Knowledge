#include "AppUser.h"

using namespace Protocol::Protobuf;

CAppUser::CAppUser()
{
}

CAppUser::~CAppUser()
{

}

void CAppUser::ProcessItem(zmsg_t* zMsg)
{
	if(nullptr == zMsg)
		return ;
	
	int nType = 0;
	switch(nType)
	{
		case 0:
			break;
		default:
			break;
	}
}


bool CAppUser::ParseWarningInfo(const char* pMsg, int nLen)
{
	if(nullptr == pMsg || 0 == nLen)
	{
		LogWarn("%s Warning information is NULL.", __FUNCTION__);
		return false;
	}

	LogWarning pLogWarn;
	pLogWarn.ParseFromArray(pMsg, nLen);
//}
//
//bool CAppUser::ParseWarnningInfo(::google::protobuf::Message* pMessage)
//{
//	if(nullptr == pMessage)
//	{
//		LogWarn("%s ::Message class  is NULL", __FUNCTION__);
//		return false;
//	}
//
//
//	
//	LogWarning*  pLogWarn = (LogWarning*) pMessage;
////	char buf[1024] ={0};
//
	int nLevel = 1;
	if(pLogWarn.has_level())
	{
		nLevel = g_ciccAlarmService->GetWarningLevel(pLogWarn.level());
	}
	std::string strAppType = pLogWarn.servicetype();

	CBuffer stBuf;
	
	GeneWarningInfo(&stBuf, &pLogWarn);
	
	g_ciccAlarmService->SendWarningInfo(nLevel, strAppType, &stBuf);
	return true;
}

bool CAppUser::GeneWarningInfo(CBuffer* pBuffer, Protocol::Protobuf::LogWarning* pLogWarnInfo)
{
	pBuffer->Clear();
	if(pLogWarnInfo == nullptr)
	{
		return false;
	}
	char szTmp[256] = { 0 };
	
	sprintf(szTmp, "\r\n\t %-*s: %ld\r\n", 15, "SerialNumber", pLogWarnInfo->serialnum());
	if(!pBuffer->Append(szTmp, strlen(szTmp)))
	{
		LogError("%s(%d) Append string error.", __FILE__, __LINE__);
		return false;
	}

	sprintf(szTmp, "\t %-*s: %s\r\n", 15,  "AppID",pLogWarnInfo->serviceid().c_str());
	if(!pBuffer->Append(szTmp, strlen(szTmp)))
	{
		LogError("%s(%d) Append string error.", __FILE__, __LINE__);
		return false;
	}

	//
	if(pLogWarnInfo->has_triggertime())
	{
		sprintf(szTmp, "\t %-*s: %ld\r\n", 15, "TriggerTime", pLogWarnInfo->triggertime());
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}
	//
	if(pLogWarnInfo->has_level())
	{
		sprintf(szTmp, "\t %-*s: %s\r\n", 15, "WarningLevel", pLogWarnInfo->level().c_str());
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}
	
	if(pLogWarnInfo->has_servicetype())
	{
		sprintf(szTmp, "\t %-*s: %s\n", 15, "ServiceType", pLogWarnInfo->servicetype().c_str());
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}

	if(pLogWarnInfo->has_description())
	{
		sprintf(szTmp, "\t %-*s: %s\n", 15, "Description", pLogWarnInfo->description().c_str());
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}

	
	if(pLogWarnInfo->has_rawlogentry())
	{
		sprintf(szTmp, "\t %-*s: ", 15, "RawLog");
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}

		if(!pBuffer->Append(pLogWarnInfo->rawlogentry().c_str(), pLogWarnInfo->rawlogentry().length()))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
		strcpy(szTmp, "\r\n");
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}

	return true;
}
