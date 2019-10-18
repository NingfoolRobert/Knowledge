#pragma once 

#include "Socket.h"


class CConnection:public CSocket
{
public:
	CConnection();
	virtual ~CConnection();
public:

	bool SendMsg(const char* pszHostIP, unsigned short nPort, const void* pszBuf, unsigned int nLength);
	//
	bool PeerMsg(char* pszBuf, int nLength);

	void Close();





private:
	
};

