/** 
 * Copyright (C) 2020 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		CppUrl.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2020-05-22
 */


#pragma once 

#include "curl.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string> 


size_t WriteGetResp(void *buffer, size_t size, size_t nmemb, void* userp);

class CppUrl 
{
public:
	CppUrl();
	virtual ~CppUrl();
public:
	virtual bool HttpsGet(const char* pszUrl, std::string& respdata);

	bool HttpsPost(const char* pszUrl, std::string& respdata);
public:
	void SetURLOpt(int nLevel, const char* pszOptional);
private:
	CURL*	m_pCurl;	
};

