#include "AppUser.h"
#include "CommonHelper.h"


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
	zframe_t* fHead = zmsg_first(zMsg);

	char szHead[32] = { 0 };
	//memcpy(szHead, fHead, zframe_size(fHead));
//	if(!isHandleMsg((char*)fHead))		//未监听业务不处理
//		return ;
//
	std::string str((char*)zframe_data(fHead), zframe_size(fHead));

	zframe_t * data = zmsg_next(zMsg);
	
	ParseWarningInfo((char*)(zframe_data(data)), zframe_size(data));
}


bool CAppUser::ParseWarningInfo(const char* pMsg, int nLen)
{
	if(nullptr == pMsg || 0 == nLen)
	{
		LogWarn("%s Warning information is NULL.", __FUNCTION__);
		return false;
	}

	LogWarning pLogWarn;
	if(!pLogWarn.ParseFromArray(pMsg, nLen))
	{
		LogError("Message Parse Serialize fail.");
		return false;
	}
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
	
//	sprintf(szTmp, "\r\n\t %-*s\t%ld\r\n", 15, "SerialNumber:", pLogWarnInfo->serialnum());
//	if(!pBuffer->Append(szTmp, strlen(szTmp)))
//	{
//		LogError("%s(%d) Append string error.", __FILE__, __LINE__);
//		return false;
//	}

	sprintf(szTmp, "\t %-*s\t%s\r\n", 15,  "AppID:",pLogWarnInfo->serviceid().c_str());
	if(!pBuffer->Append(szTmp, strlen(szTmp)))
	{
		LogError("%s(%d) Append string error.", __FILE__, __LINE__);
		return false;
	}

	//
	if(pLogWarnInfo->has_triggertime())
	{
		int64_t nTime = pLogWarnInfo->triggertime();
		char szTime[64] = { 0 };
		CommonHelper::TransferTimeInt2String(szTime, nTime);
		sprintf(szTmp, "\t %-*s\t%s\r\n", 15, "TriggerTime:", szTime);
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}
	//
	if(pLogWarnInfo->has_level())
	{
		sprintf(szTmp, "\t %-*s\t%s\r\n", 15, "WarningLevel:", pLogWarnInfo->level().c_str());
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}
	
	if(pLogWarnInfo->has_servicetype())
	{
		sprintf(szTmp, "\t %-*s\t%s\r\n", 15, "ServiceType:", pLogWarnInfo->servicetype().c_str());
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}

	if(pLogWarnInfo->has_description())
	{
		sprintf(szTmp, "\t %-*s\t%s\r\n", 15, "Description:", pLogWarnInfo->description().c_str());
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}

	
	if(pLogWarnInfo->has_rawlogentry())
	{
		sprintf(szTmp, "\t %-*s\t", 15, "RawLog:");
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
		if(!pBuffer->Append(szTmp, strlen(szTmp) + 1))
		{
			LogError("%s(%d) Append string error.", __FILE__, __LINE__);
			return false;
		}
	}

	return true;
}
