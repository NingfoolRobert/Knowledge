// 
// Call Center On Demand Product Series
// Copyright (C) 2019 CICC(Beijing.) 
// All right reserved
// 
// $$
// TODO:
// 
// *******************************************************/
/// @file Protocol.h
/// @brief 
/// @author ningbf,Beifei.Ning@cicc.com.cn
/// @version 0.1
/// @date 2019-10-24

#pragma once

#include <stdio.h>


typedef struct stHeader
{
	unsigned int	dwSerial;			//消息序列号 
	unsigned int	dwLength;			//消息长度
	unsigned int	dwType;				//消息类型
	unsigned short	wOrigin;			//消息来源 
	unsigned int	dwParam;			//消息参数 

}HEADER,*PHEADER;


#define HEADSIZE  (int)sizeof(HEADER)

//消息源值
enum 
{
	CONNECTIONTYPE_NULL					= 0,
	CONNECTIONTYPE_SUM 
};

#define REQ_MSG			0x00000000				//最高位为0; 
#define ACK_MSG			0x80000000				//32位为1 
#define CMP_MSG			0x40000000				//31位为1
