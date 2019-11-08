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

#include "blockingconcurrentqueue.h"
#include "Buffer.h"

template<class T>
class CZMQPub
{
public:
	
	CZMQPub();
	virtual ~CZMQPub();
public:
	bool Init(std::string strURI);
	
	bool PostMsg(const char* pszTopic, CBuffer* pBufMsg);

	bool PostMsg(zmsg_t* msg);

	void OnMsg();

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
	
	moodycamel::blockingconcurrentqueue<zmsg_t*>		m_listMsgPub;
};
