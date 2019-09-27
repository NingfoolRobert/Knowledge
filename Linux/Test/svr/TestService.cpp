#include "TestService.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <string.h>
using namespace std;


CTestService::CTestService()
{
	m_fp = fopen("/home/nbf/Test.log","a+");
		
}

CTestService::~CTestService()
{
	fclose(m_fp);
}

bool CTestService::OnInitialUpdate()
{
//	if(!CNetObjService::OnInitialUpdate())
//		return false;

	if(m_fp == nullptr)
		return false;
	char szTmp[100]={"OnInitialUpdate\n"};
	fwrite(szTmp,1,strlen(szTmp),m_fp);

		
	return true;
}

bool CTestService::OnTimeout(struct tm* pTime)
{
//	if(!CNetObjService::OnTimeout(pTime))
//		return false;

	if(nullptr == m_fp)
		return false;
	char szTmp[100]={"OnTimeout\n"};	
	int nwrite  = 	fwrite(szTmp, 1, strlen(szTmp), m_fp);
	return true;
}

bool CTestService::InvokeTerminate()
{
//	if(!CNetObjService::InvokeTerminate())
//		return false;
//
	if(m_fp == nullptr)
		return false;
	char szTmp[100]={"InvokeTerminate\n"};
	int nwrite = fwrite(szTmp,1,strlen(szTmp),m_fp);
	return true;
}

bool CTestService::OnTerminate()
{
//	if(!CTestService::OnTerminate())
//		return false;


	if(m_fp == nullptr)
		return false;
	char szTmp[100]={"Terminate\n"};
	fwrite(szTmp,1,strlen(szTmp),m_fp);


	return true;
}


bool CTestService::OnSecondIdle()
{
	if(nullptr == m_fp)
		return false;
	char szTmp[100]={"OnSecondIdle\n"};
	fwrite(szTmp,1,strlen(szTmp),m_fp);
	return true;
}
