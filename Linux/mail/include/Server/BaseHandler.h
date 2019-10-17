//
// Created by aurora on 6/18/19.
//

#ifndef MDS_BASEHANDLER_H
#define MDS_BASEHANDLER_H

#include "blockingconcurrentqueue.h"
#include "czmq.h"
#include <string>

using namespace std;

class CSubObj;

class BaseHandler{
public:

    BaseHandler();
    virtual ~BaseHandler();

    virtual void Start();
    virtual void Push(zmsg_t*);
    virtual void ProcessItem(zmsg_t *) = 0;

public:
	bool Bind(CSubObj* pSub);
protected:
    static void CheckQueueThread(BaseHandler* para);
    moodycamel::BlockingConcurrentQueue<zmsg_t*> m_msgs;
    bool b_isStart;
	CSubObj* m_pSubObj;
};

#endif //MDS_BASEHANDLER_H
