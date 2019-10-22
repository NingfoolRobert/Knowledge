#pragma once 
#include "czmq.h"
#include "BaseHandler.h"
#include <string>
#include "AlarmService.h"
#include "PBMessageDefine.h"
#include "Buffer.h"



class CAppUser:
	public BaseHandler
{
public:

	CAppUser();
	virtual ~CAppUser();
public:

	virtual void ProcessItem(zmsg_t * zMsg);

public:
	bool ParseWarningInfo(const char* pMsg, int nLen);

protected:
	bool GeneWarningInfo(CBuffer* pBuffer, Protocol::Protobuf::LogWarning* pLogWarnInfo);
};
