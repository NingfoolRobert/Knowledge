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
 * @date:com2020-04-21
 */



#pragma once 

typedef struct tagEventHeader 
{
	unsigned int dwType;
	unsigned int dwLength;
	unsigned int dwParam; 
}EVENTHEADER, *PEVENTHEADER;
