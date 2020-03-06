#include "NotifitionInfoMgr.h"
#include <string.h>	


CNotifitionInfoMgr::CNotifitionInfoMgr()
{
	strcpy(m_szDBAllPathName, "NotifyInfo.db");
}

CNotifitionInfoMgr::~CNotifitionInfoMgr()
{
	Terminate();
}
	
bool CNotifitionInfoMgr::Init()
{

	CreateNotifyInfoDB();
	
	CreateUserInfoTable();
	CreateGroupInfoTable();
	CreateBizInfoTable();


	return true;
}

bool CNotifitionInfoMgr::Terminate()
{
	
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	
	db_instance.close();
	return true;
}

bool CNotifitionInfoMgr::CreateNotifyInfoDB()
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	db_instance.open(m_szDBAllPathName);
	return true;
}

bool CNotifitionInfoMgr::CreateUserInfoTable()
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	char szOp[1024] = { 0 };
	sprintf(szOp, "%s", "CREATE TABLE IF NOT EXISTS tb_user_info(\
						UserID			INTEGER			PRIMARY KEY UNIQUE NOT NULL,\
						UserName		VARCHAR(64)		NOT NULL,\
						EnglishName		VARCHAR(64),\
						email			VARCHAR(128)	NOT NULL,\
						WeChatID		VARCHAR(32),\
						PhoneNum		VARCHAR(32),\
						DepartmentID	INTEGER 		NOT NULL,\
						DepartmentName  VARCHAR(64)     NOT NULL,\
						Level			INTEGER			NOT NULL);");
	int nRowChange =  db_instance.execDML(szOp);

	return nRowChange > 0;
}
	
bool CNotifitionInfoMgr::CreateGroupInfoTable()
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	char szOp[1024] = { 0 };
	sprintf(szOp, "%s", "CREATE TABLE IF NOT EXISTS tb_group_info(\
						GroupID				INTEGER			PRIMARY KEY AUTOINCREMENT NOT NULL,\
						GroupName			VARCHAR(64)		UNIQUE NOT NULL,\
						Description			VARCHAR(260)			);");

	int nRowChange =  db_instance.execDML(szOp);

	return nRowChange > 0;
	
}
	
bool CNotifitionInfoMgr::CreateBizInfoTable()
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	char szOp[1024] = { 0 };
	sprintf(szOp, "%s", "CREATE TABLE IF NOT EXISTS tb_biz_info(\
						BizID			INTEGER			PRIMARY KEY AUTOINCREMENT NOT NULL,\
						BizName			VARCHAR(64)		UNIQUE NOT NULL,\
						AffDepartmentID	INTEGER			NOT NULL);");

	int nRowChange =  db_instance.execDML(szOp);

	return nRowChange > 0;
}

bool CNotifitionInfoMgr::InsertUserInfo(const PB::UserInfo& _User)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	char szOp[1024] = { 0 };

	sprintf(szOp, "SELECT COUNT(*) FROM tb_user_info WHERE UserID = %d", _User.userid());
	int nCount = db_instance.execScalar(szOp);
	if(!nCount)
	{
		sprintf(szOp, "INSERT INTO tb_user_info(UserID, UserName, EnglishName, email, WeChatID, DepartmentID, DepartmentName, Level)VALUES(%d, \"%s\", \"%s\", \"%s\", \"%s\", %d,\"%s\", %d)", _User.userid(), _User.username().c_str(), _User.englishname().c_str(), _User.email().c_str(), _User.wechatid().c_str(), _User.departmentid(), _User.departmentname().c_str(), _User.level());

		int nRowChange =  db_instance.execDML(szOp);

		return nRowChange > 0;
	}
	
	return true;
}

bool CNotifitionInfoMgr::CreateBizTable(const char* pszBizName)
{
	if(nullptr == pszBizName || 0 == strlen(pszBizName))
		return false;
	//	
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	char szOp[1024] = { 0 };
	sprintf(szOp, "CREATE TABLE IF NOT EXISTS tb_biz_%s(\
					Row_id 		INTEGER			PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL,\
					UserID		INTEGER			DEFAULT 0,\
					GroupID		INTEGER			DEFAULT 0,\
					CC			BOOLEAN			NOT NULL,\
					Focuse      INTEGER			NOT NULL,\
					CONSTRAINT fk_user_id FOREIGN KEY (UserID) REFERENCES tb_user_info(UserID),\
					CONSTRAINT fd_group_id FOREIGN KEY (GroupID) REFERENCES tb_group_info(GroupID));", pszBizName);

	int nRowChange =  db_instance.execDML(szOp);

	return nRowChange > 0;
}
	
bool CNotifitionInfoMgr::CreateGroupTable(const char* pszGroupName)
{
	char szOp[1024] = { 0 };

	sprintf(szOp, "CREATE TABLE IF NOT EXISTS tb_group_%s(\
					UserID			INTEGER		PRIMARY KEY UNIQUE NOT NULL,\
					CONSTRAINT FOREIGN KEY fk_user_id(UserID) REFERENCES t_user_info(UserID));", pszGroupName);
	
	
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	int nRowChange =  db_instance.execDML(szOp);
	return nRowChange > 0;
}

bool CNotifitionInfoMgr::AddUser2BizTab(const PB::UserInfo& _User, const char* pszBizName)
{
	if(nullptr == pszBizName || 0 == strlen(pszBizName))
		return false;

	InsertUserInfo(_User);
	//
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();

	char szOp[1024] = { 0 };
	sprintf(szOp, "SELECT COUNT(*) FROM tb_biz_%s WHERE UserID = %d", pszBizName, _User.userid());
	int nCount = db_instance.execScalar(szOp);
	if(nCount)
	{
		return true;
	}
	
	sprintf(szOp, "INSERT INTO tb_biz_%s(UserID, CC, Focuse)VALUES(%d, %d, %d);", pszBizName, _User.userid(), _User.iscc(), (int)_User.focuslevel());
	int nRowChange =  db_instance.execDML(szOp);

	return nRowChange > 0;
}

bool CNotifitionInfoMgr::DelUserFromBizTab(const PB::UserInfo& User, const char* pszBizName)
{
	if(nullptr == pszBizName || 0 == strlen(pszBizName))
		return false;
	//
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	char szOp[1024] = { 0 };

	sprintf(szOp, "DELETE FROM tb_biz_%s WHERE UserID = %d", pszBizName, User.userid());
	int nRowChange =  db_instance.execDML(szOp);

	return nRowChange >= 0;
}

bool CNotifitionInfoMgr::AddBizInfo(const PB::BizInfo& _Biz)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	char szTableName[32] = { 0 };
	char szOp[1024] = { 0 };
	sprintf(szTableName, "tb_biz_%s", _Biz.bizname().c_str());
	
	if(!db_instance.tableExists(szTableName))	
	{
		CreateBizTable(_Biz.bizname().c_str());
		
		sprintf(szOp, "INSERT INTO tb_biz_info(BizName, AffDepartmentID, Description)VALUES(\"%s\", %d, \"%s\");", _Biz.bizname().c_str(), _Biz.affdepartment(), _Biz.description().c_str());

		return db_instance.execDML(szOp);
	}

	return true;
}

//从tb_biz_info 表中删除用户信息
bool CNotifitionInfoMgr::DelBizInfo(const PB::BizInfo& BizInfo)
{
	char szOp[1024] = { 0 };
	sprintf(szOp, "DELETE FROM tb_biz_info WHERE BizName = %s;", BizInfo.bizname().c_str());
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	int nRowChange =  db_instance.execDML(szOp);
	return nRowChange >= 0;
}


bool CNotifitionInfoMgr::AddGroup2Biz(const PB::GroupInfo& pGroup, const char* pszBizName)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();

	int nGroupId = 0;
	AddGroup2Info(pGroup, nGroupId);

	
	char szOp[1024] = { 0 };
	
	sprintf(szOp, "SELECT COUNT(*) FROM tb_biz_%s WHERE GroupID = %d", pszBizName, nGroupId);

	if(db_instance.execScalar(szOp))
	{
		return true;
	}


	sprintf(szOp, "INSET INTO tb_biz_%s(GroupID, CC, Focuse)VALUES(%d, %d, %d)",pszBizName, nGroupId, pGroup.iscc(), pGroup.focuslevel());
	return db_instance.execDML(szOp) > 0;
}

bool CNotifitionInfoMgr::DelGroupBiz(const PB::GroupInfo& pGroup, const char* pszBizName)
{
	return true;
}

bool CNotifitionInfoMgr::AddUser(const PB::UserInfo& pUser)
{
	return true;
}

bool CNotifitionInfoMgr::DelUser(const PB::UserInfo& pUser)
{
	return true;
}
	
bool CNotifitionInfoMgr::AddBizNotifyInfo(const PB::BizInfo& pBizInfo)
{
	AddBizInfo(pBizInfo); 

	for(auto i = 0; i < pBizInfo.listuserinfo_size(); ++i)
	{
		AddUser2BizTab(pBizInfo.listuserinfo(i), pBizInfo.bizname().c_str());
	}

	for(auto i = 0; i < pBizInfo.listgroupinfo_size(); ++i)
	{
		auto& pGroup = pBizInfo.listgroupinfo(i);
		AddGroupInfo(pGroup);
		AddGroup2Biz(pBizInfo.listgroupinfo(i), pBizInfo.bizname().c_str());
	}

	return true;
}
	
bool CNotifitionInfoMgr::AddGroupInfo(const PB::GroupInfo& pGroup)
{
	int nGroupId = 0;
	AddGroup2Info(pGroup, nGroupId);	
	for(auto i = 0; i < pGroup.listuserinfo_size(); ++i)
	{
		auto& pUser = pGroup.listuserinfo(i);
		AddUser2Group(pUser, pGroup.groupname().c_str());	
	}

	return true;
}

bool CNotifitionInfoMgr::DelGroupInfo(const PB::GroupInfo& GroupInfo)
{
	return true;
}
	
//根据业务名和告警级别查询通知用户信息
bool CNotifitionInfoMgr::GetNotifier(PB::BizInfo* pBiz, const char* pszBizName, int nLevel)
{
	if(nullptr == pBiz || nullptr == pszBizName || 0 == strlen(pszBizName))
		return false;
	//
	char szOp[1024] = { 0 };
	sprintf(szOp, "SELECT  tc.UserID, tc.EnglishName, tc.UserName,  t.CC, t.Focuse,tc.email,tc.WeChatID, tc.PhoneNum,tc.Level FROM tb_biz_%s t, tb_user_info tc WHERE t.UserID = tc.UserID and t.Focuse < %d ;", pszBizName, nLevel + 1);
	CppSQLite3Query query = Singleton<CppSQLite3DB>::GetInstance().execQuery(szOp);

	while(!query.eof())
	{
		auto pUser = pBiz->add_listuserinfo();
		
		pUser->set_userid(query.getIntField("UserID"));
		pUser->set_username(query.getStringField("UserName"));
		pUser->set_englishname(query.getStringField("EnglishName"));
		pUser->set_email(query.getStringField("email"));
		pUser->set_wechatid(query.getStringField("WeChatID"));
		pUser->set_iscc(query.getIntField("CC"));
		pUser->set_phonenum(query.getStringField("PhoneNum"));
		pUser->set_level(query.getIntField("Level"));
		pUser->set_focuslevel(query.getIntField("Focuse"));		
		query.nextRow();
	}
	query.finalize();
	// 
	//
	std::vector<int> listGroup;
	sprintf(szOp, "SELECT tg.GroupID, tg.GroupName, t.CC, t.Focuse,FROM tb_biz_%s t, tb_group_info tg WHERE t.GroupID = tg.GroupID AND t.Focuse < %d;", pszBizName, nLevel+1);
	query = Singleton<CppSQLite3DB>::GetInstance().execQuery(szOp);
	while(!query.eof())
	{
		auto pGroup = pBiz->add_listgroupinfo();
		pGroup->set_groupid(query.getIntField("GroupID"));
		pGroup->set_groupname(query.getStringField("GroupName"));
		pGroup->set_iscc(query.getIntField("CC"));
		pGroup->set_focuslevel(query.getIntField("Focuse"));
		query.nextRow();
	}
	query.finalize();

	for(auto i = 0; i < pBiz->listgroupinfo_size(); ++i)
	{
		auto pGroupInfo = pBiz->mutable_listgroupinfo(i);
		sprintf(szOp, "SELECT tu.UserID, tu.UserName, tu.EnglishName, tu.email, tu.PhoneNum, tu.WeChatID,  FROM tb_group_%s t, tb_user_info tu WHERE t.UserID = tu.UserID; ",pGroupInfo->groupname().c_str());
		query = Singleton<CppSQLite3DB>::GetInstance().execQuery(szOp);
		while(!query.eof())
		{
			auto pUser = pGroupInfo->add_listuserinfo();
			pUser->set_userid(query.getIntField("UserID"));
			pUser->set_username(query.getStringField("UserName"));
			pUser->set_englishname(query.getStringField("EnglishName"));
			pUser->set_email(query.getStringField("email"));
			pUser->set_wechatid(query.getStringField("WeChatID"));
			pUser->set_phonenum(query.getStringField("PhoneNum"));

			query.nextRow();
		}
		query.finalize();
	}

	return true;
}
	
bool CNotifitionInfoMgr::AddUser2Group(const PB::UserInfo& pUser, const char* pszGroupName)
{
	if(nullptr == pszGroupName || 0 == strlen(pszGroupName))
		return false ;

	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();

	char szOp[1024] = { 0 };

	char szTableName[32] = { 0 };
	sprintf(szTableName, "tb_group_%s", pszGroupName);
	if(!db_instance.tableExists(pszGroupName))
	{
		CreateGroupTable(pszGroupName);
	}

	sprintf(szOp, "SELECT COUNT(*) FROM tb_user_info WHERE UserID = %d", pUser.userid());
	
	int nCount= db_instance.execScalar(szOp);
	if(!nCount)
	{
		AddUser(pUser);			// 将用户加入tb_user_info
	}
	
	sprintf(szOp, "INSERT INTO tb_group_%s(UserID)VALUES(%d);",pszGroupName, pUser.userid());

	return db_instance.execDML(szOp) > 0;
}

bool CNotifitionInfoMgr::DelUserGroup(const PB::UserInfo& pUser, const char* pszGroupName)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();

	char szOp[1024] = { 0 };

	char szTableName[32] = { 0 };
	sprintf(szTableName, "tb_group_%s", pszGroupName);
	if(!db_instance.tableExists(pszGroupName))
	{
		return false;
	}

	sprintf(szOp, "DELETE  * FROM tb_group_%s WHERE UserID = %d", pszGroupName, pUser.userid());

	return db_instance.execDML(szOp);
}
	
bool CNotifitionInfoMgr::AddGroup2Info(const PB::GroupInfo& Group_, int& nGroupId)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();

	char szOp[1024] = { 0 };
	char szTableName[32] = { 0 };
	sprintf(szTableName, "tb_group_%s", Group_.groupname().c_str());
	if(!db_instance.tableExists(szTableName))
	{
		CreateGroupTable(Group_.groupname().c_str());

		sprintf(szOp, "INSERT INTO tb_group_info(GroupName)VALUES(\"%s\");", Group_.groupname().c_str());
		int nRow = db_instance.execDML(szOp);
		if(nRow > 0)
		{
			nGroupId = db_instance.lastRowId();
		}
		return nRow ;
	}

	sprintf(szOp, "SELECT GroupID FROM %s WHERE GroupName = \"%s\"", szTableName, Group_.groupname().c_str());
	
	nGroupId  = db_instance.execScalar(szOp);	
	return true;
}
	
bool CNotifitionInfoMgr::UpdateBizNotifyInfo(const PB::BizInfo& _BizInfo)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	return true;
}

bool CNotifitionInfoMgr::DelBizNotifyInfo(const PB::BizInfo& _BizInfo)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	return true;	
}
	
bool CNotifitionInfoMgr::UpdateUserInfo(const PB::UserInfo& User_)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();

	char szOp[1024] = { 0 };

	sprintf(szOp, "SELCET COUNT(*) FROM tb_user_info WHERE UserID = %d", User_.userid());
	
	if(db_instance.execScalar(szOp))
	{
		strcat(szOp, "UPDATE tb_user_info SET ");
		char szTmp[64] = { 0 };
		bool bFirst = true;
		if(User_.has_phonenum())
		{
			sprintf(szTmp, "PhoneNum = \"%s\"", User_.phonenum());
			bFirst = false;
			strcat(szOp, szTmp);
		}
		if(User_.has_departmentId())
		{
			if(bFirst)
			{
				sprintf(szTmp, "DepartmentID = %d", User_.departmentid());
				bFirst = false;
			}
			else 
			{
				sprintf(szTmp, ", DepartmentID = %d", User_.departmentid());
			}
			
			strcat(szOp, szTmp);
		}
		if(User_.has_level())
		{
			if(bFirst)
			{
				sprintf(szTmp, "Level = %d", User_.level());
				bFirst = false;
			}
			else 
			{
				sprintf(szTmp, ", Level = %d", User_.level());
			}

			strcat(szOp, szTmp);
		}
		
		if(User_.has_wechatid())
		{
			if(bFirst)
			{
				sprintf(szTmp, "WeChatID = \"%s\"", User_.wechatid().c_str());
				bFirst = false;
			}
			else 
			{
				sprintf(szTmp, ",WeChatID = \"%s\"", User_.wechatid().c_str());
			}

			strcat(szOp, szTmp);
		}
	
		sprintf(szTmp, " WHERE UserID = %d", User_.userid());

		strcat(szOp, szTmp);

		return db_instance.execDML(szOp);
	}

	// Insert User 
	sprintf(szOp, "INSERT INTO tb_user_info(UserID, UserName, EnglishName, email, WeChatID, DepartmentID, DepartmentName, Level)VALUES(%d, \"%s\", \"%s\", \"%s\", \"%s\", %d,\"%s\", %d)", _User.userid(), _User.username().c_str(), _User.englishname().c_str(), _User.email().c_str(), _User.wechatid().c_str(), _User.departmentid(), _User.departmentname().c_str(), _User.level());

		return  db_instance.execDML(szOp);
}
	
bool CNotifitionInfoMgr::UpdateBizUserInfo(const PB::UserInfo& User_, const char* pszBizName)
{
	CppSQLite3DB& db_instance = Singleton<CppSQLite3DB>::GetInstance();
	char szTableName[32] = { 0 };
	sprintf(szTableName, "tb_biz_%s", pszBizName);
	if(!db_instance.tableExists(szTableName))
	{
		return false;
	}

	sprintf(szOp, "SELECT COUNT(*) FROM tb_biz_%s WHERE UserID = %d", pszBizName, User_.userid());
	if(!db_instance.execScalar(szOp))
	{
		return false;
	}

	char szTmp[128] = { 0 };
	strcat(szOp, "Update tb_biz_%s SET ");
	bool bFirst = true;
	if(User_.has_iscc())
	{
		sprintf(szTmp, "CC = %d", User_.iscc());
		bFirst = false;
		strcat(szOp, szTmp);
	}

	if(User_.has_focuselevel())
	{
		if(bFirst)
		{
			sprintf(szTmp, "Focuse = %d", User_.focuslevel());
			bFirst =false;
		}
		else 
		{
			sprintf(szTmp, ", Focuse = %d", User_.focuslevel());
		}

		strcat(szOp, szTmp);
	}

	sprintf(szTmp, " WHERE UserID = %d", User_.userid());
	strcat(szOp, szTmp);
	return db_instance.execDML(szOp);
}
