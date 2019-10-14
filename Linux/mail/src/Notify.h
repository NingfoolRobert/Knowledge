#pragma once 

#include "Socket.h"
#include "json/json.h"
#include "Log.h"
#include <sys/time.h>
#include <time.h>
#include <string>
#include <string.h>
#include <vector>


/////////////////////////////////////////////////////

#define BUFLEN_USERNAME   20
#define BUFLEN_EMAIL	  36
#define MAX_PATH		  256
enum emITUint
{
	CICC_IT_UINT_TYPE_ETL,			//ETL 项目组
	

	CICC_IT_UINY_TYPE_SUM
};


//typedef struct stContactorInfo
//{
//	char	szName[BUFLEN_USERNAME];
//	char	szEmail[BUFLEN_EMAIL];
//	int		nDepartment;		//
//	int		nNo;				//工号 
//}CONTACTORINFO,*PCONTACTORINFO;

//////////////////////////////////////////////////////
class CNotify
{
public:
	CNotify();

	virtual ~CNotify();

public:
	virtual bool OnIntialUpdate(const char* pszConfigFileName);

	virtual bool GetRecvMailAddr(std::vector<std::string>& listRecvMailContactor);

	virtual bool SendNotify();

	virtual bool OnTimeout(struct tm* pTime);

protected:
	bool Connect(const char* pszAddr, int nPort);

	bool SendNotifyInfo(const char* pszInfo, int nBufLen);

	bool RecvMsg(char* pszBuf, int nBufLen);
protected:
	char	m_pszConfigFileName[256];

private:
	CSocket*  m_pSocket;
	int			m_nType;			//Notify Type 

};
