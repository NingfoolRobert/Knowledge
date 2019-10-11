#pragma once 

#include "Socket.h"


class CNotify
{
public:
	CNotify();
	virtual ~CNotify();
public:
	virtual bool OnIntialUpdate(const char* pszConfigFileName);

	virtual bool GetRecvMailAddr(std::vector<std::string>& listRecvMailContactor);

	virtual bool SendNotify();

public:
	
protected:
	bool Connect(const char* pszAddr, int nPort);

	bool SendNotifyInfo(const char* pszInfo, int nBufLen);

protected:
	char	m_pszConfigFileName[256];

private:
	CSocket*  m_pSocket;
};
