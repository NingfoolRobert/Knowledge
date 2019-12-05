
/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		MsgDefine.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-12-02
 */


#include <stdio.h>


typedef struct stMsgHeader 
{
	unsigned int	dwType;
	unsigned int	dwParam;
	unsigned short	wDst;
	unsigned short  wOrigine;
	unsigned int	dwLength;
}MSGHEADER, *PMSGHEADER;
