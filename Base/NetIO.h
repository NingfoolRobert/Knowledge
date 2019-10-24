

#pragma once
#include "Socket.h"

class CNetIO: public CSocket
{
public:
	CNetIO();
	virtual ~CNetIO();
public:

	virtual int Send(const char* pBuf, int nLength);
	
	virtual int Recv(char* pBuf, int nLength);

protected:
	
};

