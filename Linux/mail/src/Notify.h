#pragma once 

#include "Socket.h"
#include "json/json.h"
#include "Log.h"
#include <sys/time.h>
#include <time.h>
#include <string>
#include <string.h>
#include <vector>
#include "Contactor.h"
#include "Buffer.h"

/////////////////////////////////////////////////////

#define BUFLEN_USERNAME   20
#define BUFLEN_EMAIL	  36
#define MAX_PATH		  256


enum
{
	NOTIFY_TYPE_NULL,		

	NOTIFY_TYPE_EMAIL,			//邮箱 

	NOTIFY_TYPE_COUNT,
};

class CNotify
{
public:
	CNotify();

	virtual ~CNotify();

public:
	virtual bool OnInitialUpdate(const char* pszConfigFileName);

	virtual bool OnTimeout(struct tm* pTime);

	virtual bool Notify(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle, CBuffer* pBuffer);
public:
	
	void SetNotifyType(int nType) { m_nType = nType; }

	int GetNotifyType() { return m_nType; }

protected:
	bool Connect(const char* pszAddr, int nPort);

	bool SendNotifyInfo(const char* pszInfo, int nBufLen);

	int RecvMsg(char* pszBuf, int nBufLen);
	
	void Close();
protected:
	
	char	m_pszConfigFileName[256];
	int		m_nType;
private:
	CSocket*			m_pSocket;
};
