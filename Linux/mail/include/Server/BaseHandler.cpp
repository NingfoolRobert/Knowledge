//
// Created by aurora on 6/19/19.
//
#include "BaseHandler.h"
#include <thread>
#include <string.h>

void BaseHandler::Push(zmsg_t *msg) 
{
    m_msgs.enqueue(msg);
}

void BaseHandler::Start() 
{

    if (!b_isStart)
    {
        b_isStart = true;
        std::thread t1(CheckQueueThread, this);
        t1.detach();
    }
}

void BaseHandler::CheckQueueThread(BaseHandler *para) {
    if (!para)
        return;
    const int BUF_SIZE = 1000;
    zmsg_t* msgArr[BUF_SIZE];
    while (para->b_isStart)
    {
        size_t count = para->m_msgs.wait_dequeue_bulk(msgArr, BUF_SIZE);
        if (count > 0)
        {
            for (int ii = 0; ii < count; ii++)
            {
                zmsg_t* msg = msgArr[ii];
                para->ProcessItem(msg);
				zmsg_destroy(&msg);
            }
        }
    }
}

BaseHandler::BaseHandler() 
{
}

BaseHandler::~BaseHandler() 
{

}

//bool BaseHandler::GetSubObjTopic(tinyxml2::XMLElement* pNode)
//{
//	if(nullptr == pNode)
//	{
//		return false;
//	}
//	
//	return true;
//}

//bool BaseHandler::AddSubObjTopic(std::string strTopic)
//{
//	if(0 == strTopic.length())
//	{
//		return false;
//	}
//	
//	m_listTopic.insert(strTopic);
//	
//	return true;
//}
//int BaseHandler::isHandleMsg(const char* pszTopic)
//{
//	if(nullptr == pszTopic || 0 == strlen(pszTopic))
//		return 0;
//	for(auto it = m_listTopic.begin(); it != m_listTopic.end();++it)
//	{
//		std::string str = *it;
//		if(strstr(str.c_str(), pszTopic) != nullptr)
//			return 1;
//	}
//	return 0;
//}
