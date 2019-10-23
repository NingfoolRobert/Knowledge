#include "AlarmService.h"
#include "AppUser.h"

void CAlarmService::Test()
{

	using namespace Protocol::Protobuf;
	
//	required int64 serialNum = 1;        // 报警序列号
//	required string serviceID = 2;      // 被监控者ID
//	optional int64 triggerTime = 3;      // 报警触发时间
//	optional string level = 4;           // 报警等级
//	optional string serviceType = 5;    // 业务类型 FIX, HS, JZ, etc.
//	optional string infoType = 6;     // SYS, BUS
//	optional string clientID = 7;        // 客户参考号, 用于表示FIX session或其他存在多客户的业务
//	optional string referenceNum = 8;    // 参考号, 用于存储业务代码, 如ClOrdId, ContractNum等
//
//	optional string rawLogEntry = 9;    // 原始报警日志记录
//	optional string description = 10;

	LogWarning WarnInfo;
	WarnInfo.set_serialnum(123);
	WarnInfo.set_serviceid("AppID");
	WarnInfo.set_triggertime(20191023142836);
	WarnInfo.set_level("ERROR");
	WarnInfo.set_servicetype("FIX");
	WarnInfo.set_infotype("BUS");
	WarnInfo.set_clientid("1234");
	WarnInfo.set_referencenum("501166411");
	WarnInfo.set_rawlogentry("[Error] create business fail.");
	WarnInfo.set_description("下单失败");

	int nLen = WarnInfo.ByteSize();
	std::vector<char> buf;
	buf.resize(nLen);
	if(!WarnInfo.SerializeToArray(&buf[0], nLen))
	{
		LogError("Serialize fail.");	
		return ;
	}
	
	std::string Topic("LOG^");
	zmsg_t * msg = zmsg_new();
	zframe_t * TopicFrame  = zframe_new(Topic.c_str(), Topic.length() + 1);
	zmsg_append(msg, &TopicFrame);
	zframe_t * WarnFrame = zframe_new(&buf[0], nLen);
	zmsg_append(msg, &WarnFrame);
//
	CAppUser	App;
	App.ProcessItem(msg);
//	App.ParseWarningInfo(pBuf, WarnInfo.ByteSize());
	zmsg_destroy(&msg);


	LogInfo("Test Error...");	
}
