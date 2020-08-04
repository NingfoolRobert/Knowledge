/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		GFunc.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-07-11
 */


#pragma once 


unsigned int Str2HostIP(const char* pszIP);


unsigned int Net2HostIP(unsigned int dwNetIP);


char* HostIP2Str(unsigned int dwHostIP, char* pszIP);
