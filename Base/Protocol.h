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
	unsigned int	dwLength;
	unsigned short	wOrigin;
	unsigned short	wParam;
}HEADER,*PHEADER;


#define HEADSIZE  sizeof(HEADER)
