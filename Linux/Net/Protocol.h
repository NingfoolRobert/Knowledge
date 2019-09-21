#pragma once


#include <stdio.h>

typedef struct stHeader
{
	unsigned int	uLength;
	unsigned int	uSerial;		//消息序列号
	unsigned short  uOrigin;		//消息源 
	unsigned short  uReserve;		//保留
}HEADER, *PHEADER;
