//
// Created by aurora on 6/18/19.
//

#ifndef MDS_BASEHANDLER_H
#define MDS_BASEHANDLER_H

#include "blockingconcurrentqueue.h"
#include "czmq.h"
#include <string>

using namespace std;

class BaseHandler{
public:

    BaseHandler(string, string, string);
    virtual ~BaseHandler();

    virtual void Start();
    virtual void Push(zmsg_t*);
    virtual void ProcessItem(zmsg_t *) = 0;

protected:
    static void CheckQueueThread(BaseHandler* para);
    moodycamel::BlockingConcurrentQueue<zmsg_t*> m_msgs;
    bool b_isStart;
    string str_group;
    string str_handler;
    string str_type;


};

#endif //MDS_BASEHANDLER_H
