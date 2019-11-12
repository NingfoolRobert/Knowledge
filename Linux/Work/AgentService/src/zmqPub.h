/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		zmqPub.h
 * @brief:		Pub Message moudle
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-11-08
 */

#pragma once 

#include <czmq.h>
#include <mutex>
#include <condition_variable> 
#include <atomic>
#include <queue> 

#include "blockingconcurrentqueue.h"
#include "Buffer.h"


//template<class T>
class CZMQPub
{
public:
	
	CZMQPub(void);
	virtual ~CZMQPub(void);
public:
	bool Init(std::string strURI);
	
	bool PostMsg(const char* pszTopic, CBuffer* pBufMsg);

	bool PostMsg(zmsg_t* msg)
	{
		if(nullptr == msg)
		{
			return false;
		}
		//
		std::unique_lock<std::mutex> locker(m_clsLock);
		m_listMsgPub.push(msg);
		m_condPub.notify_one();
		return true;
	}

	void OnMsg()
	{
		while(!m_bStop)
		{
			//std::unique_lock<std::mutex>  locker(m_clsLock);
			m_clsLock.lock();
			//size_t nSize = m_listMsgPub.size_approx();
			//m_condPub.wait(locker, [&](){ return m_listMsgPub.size_approx(); });
		//	m_condPub.wait(locker, [&]() -> bool{return !m_listMsgPub.empty();});
			std::vector<zmsg_t*> listMsg;
		//	listMsg.resize(m_listMsgPub.size_approx());
		//	int nCount = m_listMsgPub.wait_dequeue_bulk(&listMsg[0], nSize);
		
			while(!m_listMsgPub.empty())
			{
				zmsg_t* pmsg = m_listMsgPub.front();
				m_listMsgPub.pop();
				listMsg.push_back(pmsg);
			}
			m_clsLock.unlock();
			//locker.unlock();
			//
			for(int i = 0; i < listMsg.size(); ++i)
			{		
				auto pmsg =	listMsg[i];
				if(pmsg == nullptr)
					continue;
				if(nullptr == m_zsock)
					continue;
				zmsg_send(&pmsg,m_zsock);
				zmsg_destroy(&pmsg);
				pmsg = nullptr;
			}
			listMsg.clear();
		}
		usleep(1000000);
	}

	void Terminate();

	void GetPubInfo(CBuffer* pBufInfo);

	unsigned long GetPubSerialNum(int nIncrement = 1);

private:
	std::mutex						m_clsSerialNum;
	unsigned long					m_ulSerialNum;		//Pub源消息序列号
private:
	zsock_t*						m_zsock;
private:
	bool							m_bStop;
	std::mutex						m_clsLock;
	std::condition_variable			m_condPub;
	std::queue<zmsg_t*>				m_listMsgPub;	
//	moodycamel::BlockingConcurrentQueue<zmsg_t*>		m_listMsgPub;
};
