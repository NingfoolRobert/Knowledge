#include <stdio.h>
#include <stdlib.h>
#include "blockingconcurrentqueue.h"

#include "czmq.h"



class CUserObject
{
public:
	CUserObject();
	virtual ~CUserObject();
public:
//	bool	Push(zmg_t* zMsg);
//	virtual bool OnMsg(zmsg_t* zMsg);


private:

//	moodycamel::BlockingConcurrentQueue<zmg_t*>	m_listMsg;

};
