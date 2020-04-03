/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxEventDef.h
 * @brief:xxxEventDef 
 * @author:xxxEventDefnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-04-03
 */



#pragma once 
#include <cstdio> 
#include <cstdlib> 


typedef struct tagEventHeader 
{
	unsigned int dwType;
	unsigned int dwParam;	
	unsigned int dwLength;
}EVENTHEADER, *PEVENTHEADER;


typedef struct tagTimerHeader 
{
	unsigned int dwTimerID;
	unsigned int dwLength;
	int64_t		 pOwner;
	int64_t		 llTimerOver;
}TIMERHEADER, *PTIMEHEADER;

