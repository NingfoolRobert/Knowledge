#include "zmqPub.h"

#include <thread>
#include "Log.h"
#include <vector>


//template<class T>
CZMQPub::CZMQPub(void)
{
	m_bStop = false;
	m_ulSerialNum = 0;
	m_zsock = nullptr;
}

//template<class T>
CZMQPub::~CZMQPub(void)
{
	if(nullptr != m_zsock)
	{
		Terminate();
	//	zsock_destroy(&m_zsock);
	}

}

//template<class T>
bool CZMQPub::Init(std::string strURI)
{
	m_zsock = zsock_new(ZMQ_PUB);
	if(nullptr == m_zsock)
	{
		return false;
	}

	zsock_set_hwm(m_zsock, 0);
	zsock_set_sndbuf(m_zsock, 65535);
	zsock_set_tcp_keepalive(m_zsock, 1);
	zsock_set_tcp_keepalive_idle(m_zsock, 600);
	zsock_set_tcp_keepalive_intvl(m_zsock, 10);
	zsock_set_tcp_keepalive_cnt(m_zsock, 6);
	int nPort = zsock_bind(m_zsock, strURI.c_str());
	
	if(nPort <= 0)
		return false;

	std::thread tr1(&CZMQPub::OnMsg, this);
	return true;
}

//template<class T>
bool CZMQPub::PostMsg(const char* pszTopic, CBuffer* pBufMsg)
{
	if(nullptr == pszTopic || pBufMsg == nullptr)
		return false;

	if(m_bStop)
	{
		return false;
	}

	zframe_t* zfTopic = zframe_new(pszTopic, strlen(pszTopic) + 1);
	zframe_t* zfData = zframe_new(pBufMsg->GetBufPtr(), pBufMsg->GetBufLen());
	zmsg_t* msg = zmsg_new();
	zmsg_append(msg, &zfTopic);
	zmsg_append(msg, &zfData);

	std::unique_lock<std::mutex>  locker(m_clsLock);
	m_listMsgPub.push(msg);
	m_condPub.notify_one();
	return true;
}

//template<class T>
//bool CZMQPub::PostMsg(zmsg_t* msg)
//{
//	if(nullptr == msg)
///	{
//		return false;
//	}
//	//
//	std::unique_lock<std::mutex> locker(m_clsLock);
//	m_listMsgPub.enqueue(msg);
//	m_condPub.notify_one();
//	return true;
//}

//template<class T>
//void CZMQPub::OnMsg()
//{
//	while(!m_bStop)
//	{
//		std::unique_lock<std::mutex>  locker(m_clsLock);
//		size_t nSize = m_listMsgPub.size_approx();
//		m_condPub.wait(locker, [&](){ return m_listMsgPub.size_approx(); });
//		std::vector<zmsg_t*> listMsg;
//		listMsg.resize(m_listMsgPub.size_approx());
//		int nCount = m_listMsgPub.wait_dequeue_bulk(&listMsg[0], nSize);
//		locker.unlock();
//		//
//		for(int i = 0; i < nCount; ++i)
//		{		
//			auto pmsg =	listMsg[i];
//			if(pmsg == nullptr)
//				continue;
//			zmsg_send(&pmsg,m_zsock);
//			zmsg_destroy(&pmsg);
//			pmsg = nullptr;
//		}
//		listMsg.clear();
//	}
//}

//template<class T>
void CZMQPub::Terminate()
{
	m_bStop = true;
	std::unique_lock<std::mutex> locker(m_clsLock);
	m_condPub.notify_all();
	if(m_zsock == nullptr)
		return ;
//	std::vector<zmsg_t*> listMsg;
//	listMsg.resize(m_listMsgPub.size_approx());
//	int ncount = m_listMsgPub.wait_dequeue_bulk(&listMsg[0], m_listMsgPub.size_approx());
//	for(int i = 0; i < ncount; ++i)
//	{
//		auto pMsg = listMsg[i];
//		zmsg_destroy(&pMsg);
//		pMsg = nullptr;
//	}
	//
	zsock_destroy(&m_zsock);
	m_zsock = nullptr;
}

//template<class T>
void CZMQPub::GetPubInfo(CBuffer* pBufInfo)
{
	if(pBufInfo == nullptr)
		return ;
	char szTmp[256] = { 0 };
	sprintf(szTmp, "SerialNUM: %ld, MsgCount: %d", m_ulSerialNum, m_listMsgPub.size());
	if(!pBufInfo->Append(szTmp, strlen(szTmp)))
	{
		LogError("%s(%d) Append Data fail.", __FILE__, __LINE__ );
	}
}
//template<class T>
unsigned long CZMQPub::GetPubSerialNum(int nIncrement/* = 1*/)
{
	std::unique_lock<std::mutex> locker(m_clsSerialNum);
	m_ulSerialNum += nIncrement;
	return m_ulSerialNum;
}
