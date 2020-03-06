/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxNotifitionInfoMgr.h
 * @brief:xxxNotifitionInfoMgr 
 * @author:xxxNotifitionInfoMgrnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-03-04
 */

#pragma once 


#include "CppSQLite3.h"
#include "Singleton.h"
#include "../proto/msg/NotifyInfo.pb.h"



#include <vector> 
#include <set>

class CNotifitionInfoMgr 
{
public:
	CNotifitionInfoMgr();
	virtual ~CNotifitionInfoMgr();
public:
	
	bool Init();

	bool Terminate();
public:
	bool CreateNotifyInfoDB();

	bool CreateUserInfoTable();
	
	bool CreateBizInfoTable();

	bool CreateGroupInfoTable();

	bool CreateGroupTable(const char* pszGroupName);

	bool InsertUserInfo(const PB::UserInfo&  pUserInfo);

	bool CreateBizTable(const char* pszBizName);

	bool AddUser2BizTab(const PB::UserInfo& pUserInfo, const char* pszBizName);

	bool DelUserFromBizTab(const PB::UserInfo& pUser, const char* pszBizName);

	bool AddBizInfo(const PB::BizInfo& pBizInfo);

	bool DelBizInfo(const PB::BizInfo& pBizInfo);

	bool AddGroup2Biz(const PB::GroupInfo& pGroup, const char* pszBizName);

	bool DelGroupBiz(const PB::GroupInfo& pGroup, const char* pszBizName);

	bool AddUser(const PB::UserInfo& pUser);

	bool DelUser(const PB::UserInfo& pUser);

	bool AddGroupInfo(const PB::GroupInfo& pGroup);

	bool DelGroupInfo(const PB::GroupInfo& GroupInfo);

	bool AddUser2Group(const PB::UserInfo& pUser, const char* pszGroupName);

	bool DelUserGroup(const PB::UserInfo& pUser, const char* pszGroupName);

	bool AddGroup2Info(const PB::GroupInfo& GroupInfo, int& nGroupId);

	bool UpdateUserInfo(const PB::UserInfo& User_);

	bool UpdateBizUserInfo(const PB::UserInfo& User_, const char* pszBizName);
public:
	bool AddBizNotifyInfo(const PB::BizInfo& pBizInfo);

	bool UpdateBizNotifyInfo(const PB::BizInfo& _BizInfo);

	bool DelBizNotifyInfo(const PB::BizInfo& _BizInfo);
public:
	
	bool GetNotifier(PB::BizInfo* pBiz, const char* pszBizName, int nLevel);
private:
	
	char			m_szDBAllPathName[256];
};


