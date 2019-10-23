//
// Created by aurora on 6/18/19.
//

#ifndef MDS_BASEHANDLER_H
#define MDS_BASEHANDLER_H

#include "blockingconcurrentqueue.h"
#include "czmq.h"
#include <string>
#include <set> 

//#include "tinyxml2.h"

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
	
//	bool GetSubObjTopic(tinyxml2::XMLElement* pNode);

//	bool AddSubObjTopic(std::string strTopic);

//	int isHandleMsg(const char* pszTopic);

protected:
    static void CheckQueueThread(BaseHandler* para);
    moodycamel::BlockingConcurrentQueue<zmsg_t*> m_msgs;
    bool b_isStart;
	CSubObj* m_pSubObj;
	std::set<std::string> m_listTopic;		//订阅Topic 对象 
};

#endif //MDS_BASEHANDLER_H
