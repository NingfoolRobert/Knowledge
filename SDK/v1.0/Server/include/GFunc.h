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
#include <memory> 
#include <sys/stat.h>
#include "GlobalVar.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"



bool GlobalBegin(const char* pszSimpleName);

bool GlobalEnd();

bool GetWorkPath(char* pszWorkPath);

bool AddFilePath(char* pszFileName);

//"192.168.1.1" =>  unsigned int 
unsigned int IPStr2Host(const char* pszIP);
//unsigned int  => "192.168.1.0"
char* IPHost2Str(char* pszIP, unsigned int dwIP);
// net unsigned int => "192.168.1.0"
char* IPNet2Str(char* pszIP, unsigned int dwNetIP);
//"192.168.1.0" => net unsigned int 
unsigned int IPStr2Net(const char* pszIP);

//获取本地IPlist
bool GetLocalIPList(std::vector<std::string> &listIP);

//sockaddr, 获取IP：Port 
bool GetIPAndPort(unsigned int &dwHostIP, int& nPort, struct sockaddr_in* pSockAddr);

bool HostIP2Str(unsigned int dwHostIP, char* pszIP);

unsigned int String2HostIP(const char* pszIP);
//压缩
bool Compress(char* pszDst, int nDstLen, const char* pszSrc, int nSrcLen);
//解压缩
bool UnCompress(char* pszDst, int nDstLen, const char* pszSrc, int nSrcLen);

bool GetXMLConfigString(const char* pszNode, const char* pszAttr, const char* pszDefault,  char* pszValue);

bool CreateAllPath(const char* pszAllDir);

bool GetCurrentPath(char* pszDirName);

bool GetXMLAttrString(rapidxml::xml_node<>* pNode,  const char* pszEntry, const char* pszDefault, char* pszValue);

int  GetXMLAttrInt(rapidxml::xml_node<>* pNode, const char* pszEntry, const int nDefault);

time_t GetFileModTime(const char* pszFileName);
