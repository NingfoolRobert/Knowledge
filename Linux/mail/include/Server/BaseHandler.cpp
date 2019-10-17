//
// Created by aurora on 6/19/19.
//
#include "BaseHandler.h"
#include <thread>


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

