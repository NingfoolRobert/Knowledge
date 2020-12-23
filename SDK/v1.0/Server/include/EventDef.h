/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:EventDef.h
 * @brief:EventDef 
 * @author:nbf,nbf0537@163.com
 * @version:1.0
 * @date:2020-04-21
 */



#pragma once 

typedef struct tagEventHeader 
{
	unsigned int	dwType;
	unsigned int	dwLength;
	unsigned int	dwParam; 
	unsigned long	ulParam;
}EVENTHEADER, *PEVENTHEADER;
