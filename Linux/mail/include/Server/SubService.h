#include <zmq.h>

#include <stdio.h>
#include <stdlib.h>

class CSubService
{
public:
	CSubService();
	virtual ~CSubService();
public:
	virtual bool OnInitialUpdate();

	virtual bool OnTimeout();
	
public:
	int		m_nPort;
	
private:

};
