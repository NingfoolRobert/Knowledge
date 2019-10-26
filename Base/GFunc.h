/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		GFunc.h
 * @brief:		服务内部调用公共函数
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-25
 */

#pragma once

#include <string.h>
#include <unistd.h>
#include <vector>

#include "GlobalVar.h"


bool GlobalBegin(const char* pszSimpleName);

bool GlobalEnd();

bool GetWorkPath(char* pszWorkPath);

bool AddFilePath(char* pszFileName);

//"192.168.1.1" =>  unsigned int 
unsigned int IPString2Dword(const char* pszIP);
//unsigned int  => "192.168.1.0"
char* IPDWORD2String(char* pszIP, unsigned int dwIP);
// net unsigned int => "192.168.1.0"
char* IPNetDWORD2String(char* pszIP, unsigned int dwNetIP);
//"192.168.1.0" => net unsigned int 
unsigned int IPNetString2DWORD(const char* pszIP);

//获取本地IPlist
bool GetLocalIPList(std::vector<std::string> &listIP);

//sockaddr, 获取IP：Port 
bool GetDWORDNetIPAndPort(unsigned int &dwNetIP, int& nPort, struct sockaddr* pSockAddr);


