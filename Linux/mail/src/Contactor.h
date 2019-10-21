#pragma once

#include <vector>
#include <unordered_map> 
#include <map>
#include <memory>
#include <string.h>



#include "CiccAtomLock.h"

#define BUFLEN_TYPE_NAME		20
#define BUFLEN_TYPE_EMAIL		36



// 报警级别 
#define ALARM_LEVEL_TYPE_WARNNING			0x00000001
#define ALARM_LEVEL_TYPE_ERROR				0x00000002
#define ALARM_LEVEL_TYPE_FATAL				0x00000004 

//业务类型 
#define ALARM_BUSINESS_TYPE_FIX				0x00000001				//fix 
#define ALARM_BUSINESS_TYPE_BB				0x00000002				//银行间
#define ALARM_BUSINESS_TYPE_HS				0x00000004				//
#define ALARM_BUSINESS_TYPE_JZ				0x00000008				//
//#define ALARM_BUSINESS_TYPE_FIX				0x00000010				//
//#define ALARM_BUSINESS_TYPE_FIX				0x00000020				// 
//#define ALARM_BUSINESS_TYPE_FIX				0x00000040				// 


// Notify对象 
typedef struct stContactor
{
	int		nNO;			//工号
	char	szName[BUFLEN_TYPE_NAME];
	char	szEmail[BUFLEN_TYPE_EMAIL];
	int		nDepartment;


	int		nLevel;
	std::vector<std::string>  listFocus;
	stContactor()
	{
		memset(szName, 0, sizeof(szName));
		memset(szEmail, 0, sizeof(szEmail));
		listFocus.clear();
	}
	stContactor(int _nNo, const char* pszName, const char* pszEmail, int _nDepartment):nNO(_nNo),nDepartment(_nDepartment)
	{
		strcpy(szName,  pszName);
		strcpy(szEmail, pszEmail);
	}

	stContactor(stContactor* pInfo)
	{
		nNO				= pInfo->nNO;
		nDepartment		= pInfo->nDepartment;
		nLevel			= pInfo->nLevel;
		memcpy(szName, pInfo->szName, strlen(pInfo->szName) + 1);
		memcpy(szEmail, pInfo->szEmail, strlen(pInfo->szEmail) + 1);
		listFocus.clear();
		listFocus.swap(pInfo->listFocus);
	}
	bool isNotify(int nLvl)
	{
		if(nLevel <=  nLvl)
			return true;
		return false;
	}
}CONTACTORINFO,*PCONTACTORINFO;

typedef std::shared_ptr<CONTACTORINFO> CONTACTORPtr;


class CContactor
{
public:
	CContactor();
	virtual ~CContactor();
public:
	bool Init(const char* pszFileName);

	bool GetNotifyList(std::vector<CONTACTORPtr>& listUser, int nAlarmLevel, std::string strType);

	bool Add(PCONTACTORINFO pInfo);

	bool Del(PCONTACTORINFO pInfo);

//	bool OnTimeout(struct tm* pTime);

	CONTACTORPtr  Find(PCONTACTORINFO pUser);
	
	void GetNotifyUser(const char* pszConfigureName);

	int	GetContatorCount();
protected:
	bool  UpdateContactorInfo();
private:
	char												m_szContactorFileName[256];
	time_t												m_tLastModifyTime;

	CICCTools::AtomLock									m_clsLock;	
	std::unordered_map<int, CONTACTORPtr>				m_listContactor;
	std::map<std::string, std::vector<CONTACTORPtr>* >	m_mapContactor;
};
