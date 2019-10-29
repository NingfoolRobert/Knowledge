#include "NetClient.h"
#include "GFunc.h"
#include "UserObject.h"
#include "NetService.h"
#include "ThreadPool.h"
#include "GFunc.h"
#include "Log.h"

#include <functional>
#include <algorithm>
#include <sys/epoll.h>


static void RecvThread(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
		return ;
	pNetClient->RecvThread();
}
static void SendThread(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
		return ;
	pNetClient->SendThread();
}
static void ProcessThread(CNetClient* pNetClient)
{
	if(nullptr == pNetClient)
		return ;
	pNetClient->ProcessMsg();
}

CNetClient::CNetClient()
{
	m_dwRecvSerial = 0;
	m_dwSendSerial = 0;
	m_pbufRecv = nullptr;
	m_pbufSend = nullptr;
}

CNetClient::~CNetClient()
{
	
}


bool CNetClient::OnInitialUpdate(CNetService* pNetService,int fd)		//初始化Socket, UserObject, 
{	
	if(pNetService == nullptr || fd <= 0)
	{
		LogError("%s(%d) NetService is NULL or fd <= 0.", __FILE__, __LINE__);
		return false;
	}
	Attach(fd);

//	char szAddress[32] = { 0 };

	struct sockaddr_in PeerAddr;
	socklen_t socklen = sizeof(PeerAddr);
	GetPeerName((struct sockaddr*)&PeerAddr, &socklen);

	GetIPAndPort(m_dwIP, m_nPort, &PeerAddr);

	return true;
}

bool CNetClient::OnTimeOut(struct tm* pTime)
{
	if(nullptr == pTime)
	{
		return false;
	}
	return true;
}


bool CNetClient::OnSecondIdle()
{
	m_pUserObj->OnSecondIdle();
	return true;
}


bool CNetClient::OnTerminate()
{
	CBuffer* pBuf = nullptr;
	{

		CAutoLock locker(&m_clsRecv);
		while(!m_listRecvMsg.empty())
		{
			pBuf = m_listRecvMsg.front();
			g_pBufferMgr->ReleaseBuffer(pBuf);
			m_listRecvMsg.pop();
		}
		g_pBufferMgr->ReleaseBuffer(m_pbufRecv);
	}

	{
		CAutoLock locker(&m_clsSend);
		while(!m_listSendMsg.empty())
		{
			pBuf = m_listSendMsg.front();
			g_pBufferMgr->ReleaseBuffer(pBuf);
			m_listSendMsg.pop();
		}
		g_pBufferMgr->ReleaseBuffer(m_pbufSend);
	}

	return true;
}

void CNetClient::OnRecv()		
{
	//一次最多收缓冲区大小的数据 
	//auto task  = std::bind(&CNetClient::RecvThread, this);
	//PostEvent(RecvThread, this);
	//Enqueue(RecvThread, this);
	std::thread tr1(std::mem_fn(&CNetClient::RecvThread), this);
}

void CNetClient::OnSend()
{
	// task  add in threadpool
	auto task  = std::bind(&CNetClient::SendThread, this);

//postEvent(SendThread, this);
	//Enqueue(SendThread, this);
	std::thread tr1(std::mem_fn(&CNetClient::SendThread), this);
}

void CNetClient::OnClose()
{
	while(!m_listRecvMsg.empty())
	{
	//		auto task = std::bind(&CNetClient::ProcessMsg, this);
	//	PostEvent(ProcessThread, this);
	//	Enqueue(ProcessThread, this);
	
	std::thread tr1(std::mem_fn(&CNetClient::ProcessMsg), this);
	}
	
	OnBreak();
	
	OnTerminate();
}

void CNetClient::SendThread()				//发送消息线程
{
	while(true)
	{
		if(nullptr == m_pbufSend && m_listSendMsg.empty())
		{
			UpdateEventType(EPOLLIN | EPOLLET);
			return ;
		}
		// 发送当前待发送的数据为空，从消费队列中取出新的buf
		if(nullptr == m_pbufSend)
		{
			while(!m_listSendMsg.empty())
			{
				m_pbufSend = m_listSendMsg.front();
				if(m_pbufSend == nullptr)
				{
					m_listSendMsg.pop();
					continue;
				}
				PHEADER pHeader = (PHEADER) m_pbufSend->GetBufPtr();

				//待发送数据结构内容不为nullptr
				if(pHeader == nullptr)
				{
					m_listSendMsg.pop();
					m_pbufSend = nullptr;
					continue;
				}
				else 
				{
					//重置待发送数据特性
					m_dwSendBlockSize = pHeader->dwLength;
					m_dwSendLeft = m_dwSendBlockSize;
					m_dwSendOver = 0;
					m_listSendMsg.pop();		//从队列中取出 
					break;
				}	
			}

		}
		//	
		char* pSendHead = m_pbufSend->GetBufPtr() + m_dwSendOver;
		m_dwSendOver += CNetIO::Send(pSendHead, m_dwSendLeft);
		
		m_dwSendLeft = m_dwSendBlockSize - m_dwSendOver;
			//当前写缓冲区已写满
			if(errno == EAGAIN)
			{
				return ;
			}
			//发送下一条消息
			if(m_dwSendLeft == 0)
			{
				//TODO 回收m_pBufSend;
				g_pBufferMgr->ReleaseBuffer(m_pbufSend);
		 		m_pbufSend = nullptr;
			}
	}
}

void CNetClient::RecvThread()				//接受线程 
{
	std::vector<char> szTmp;
	szTmp.resize(1024);

	if(m_pbufRecv == nullptr)
	{
		//TODO 申请内存空间 
		m_pbufRecv = g_pBufferMgr->GetBuffer(256, __FILE__, __LINE__);
		if(m_pbufRecv == nullptr)
		{
			return ;
		}
	}
	
	int nRecvLen = 0;
	while(true)
	{
		nRecvLen = Recv(&szTmp[0], 1024);
		if(errno == EAGAIN)
		{
			//内存空间中的数据接收完成 
			break;
		}
		//Append Data buffer 
		if(!m_pbufRecv->Append(&szTmp[0], nRecvLen))
		{
			LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
			break;
		}
	}

	nRecvLen = 0;
	while(true)
	{
		//当接收到的数据消息头不满足一个消息头的长度时
		if((m_pbufRecv->GetBufLen() - nRecvLen) < (int)sizeof(HEADER))
		{
			return ;
		}

		
		PHEADER pHeader = (PHEADER)(m_pbufRecv->GetBufPtr() + nRecvLen);
		//TODO 基于消息发送校验准则校验数据对象
		if(!IsBindUserObject())
		{
			if(m_dwRecvSerial == 0) 
			{
				//TODO create UserObject 
				if((pHeader->dwType & ACK_MSG) || (pHeader->wOrigin == CONNECTIONTYPE_NULL || pHeader->wOrigin >= CONNECTIONTYPE_SUM))
				{
					nRecvLen++;
		 			continue;
				}
				//创建 对象成功
				if(!m_pNetSercie->OnNetMsg(this, pHeader))
		 		{
			//		m_dwRecvSerial++;
		 			continue;
				}			
			}
		}
		else 
		{
			if(pHeader->wOrigin != m_wOrigin)
			{
				nRecvLen++;
				continue;
			}
		}
		// 当一条消息全部收入完成时	
		if((m_pbufRecv->GetBufLen() - (unsigned int)nRecvLen) > ((unsigned int)sizeof(HEADER) + pHeader->dwLength))
		{
			//正常消息加入到消息队列中
	
			CBuffer* pBuf = g_pBufferMgr->GetBuffer((int)(sizeof(HEADER) + pHeader->dwLength), __FILE__, __LINE__);//nullptr;		//TODO 申请内存空间 
			if(nullptr == pBuf)
			{
				return ;
			}			
			//
			if(!pBuf->Append(pHeader, pHeader->dwLength + sizeof(HEADER)))
			{
				LogError("%s(%d)  Append Data fail.", __FILE__, __LINE__);
				return ;
			}
			
			m_listRecvMsg.push(pBuf);
			nRecvLen += sizeof(HEADER) + pHeader->dwLength;
			m_dwSendSerial++;	
		}
	}

	CBuffer buf;
	if(!buf.Append(m_pbufRecv->GetBufPtr() + nRecvLen, m_pbufRecv->GetBufLen() - nRecvLen))
	{
		LogError("%s(%d) Append Data Error.", __FILE__, __LINE__);
		return ;
	}
	m_pbufRecv->Exchange(buf);

	//数据由消费线程消费
	auto task = std::bind(&CNetClient::ProcessMsg,this);
//	PostEvent(ProcessThread, this);
	//Enqueue(ProcessThread,this);

	std::thread tr1(std::mem_fn(&CNetClient::ProcessMsg), this);
}

bool CNetClient::OnMsg(PHEADER pMsg)		//消息到达网络端口
{
	if(nullptr == pMsg || nullptr == m_pUserObj)
	{
		return false;
	}

	return m_pUserObj->OnMsg(pMsg);
}

bool CNetClient::OnConnect()				//链接消息网络接口
{
	if(nullptr == m_pUserObj)
	{
		return false;
	}
	if(m_dwRecvSerial == 0)
	{
		m_pUserObj->OnConnect();	
	}
	
	return true;
}

bool CNetClient::OnBreak()					//网络断开
{
	if(nullptr == m_pUserObj)
	{
		LogWarn("UserObj is NULL.IP:Port = %s:%d", m_szIP, m_nPort);
	}
	else 
	{
		m_pUserObj->OnClose();
	}
	UpdateEventType(EPOLLCLOSE);
	return true;
}

bool CNetClient::SendMsg(PHEADER pMsg)
{
	if(nullptr == pMsg)
	{
		return false;
	}
	//
	CBuffer* pBuf = g_pBufferMgr->GetBuffer(pMsg->dwLength + sizeof(HEADER), __FILE__, __LINE__);
	if(nullptr == pBuf)
	{
		return false;
	}

	if(!pBuf->Append(pMsg, (int)sizeof(HEADER) + pMsg->dwLength))
	{
		LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
		return false;
	}
	
	SendMsg(pBuf);	
	return true;
}

bool CNetClient::SendZipMsg(PHEADER pMsg)
{
	if(nullptr == pMsg)
	{
		return false;
	}

	if(pMsg->dwLength <= 128)
	{
		SendMsg(pMsg);
		return true;
	}

	//TODO 压缩函数调用 

	CBuffer bufZip;
	bufZip.ExpandTo(sizeof(HEADER) + pMsg->dwLength);
	
	if(!bufZip.Append(pMsg, sizeof(HEADER)))
	{
		LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
		return false;
	}
	int nCompressLen = 0;
	if(!Compress(bufZip.GetBufPtr(), nCompressLen, (char*)(pMsg + 1), pMsg->dwLength))
	{
		LogWarn("%s(%d) Compress Message fail.", __FILE__, __LINE__);
		SendMsg(pMsg);
		return false;
	}
	
	//
	CBuffer* pBuf = g_pBufferMgr->GetBuffer(nCompressLen + sizeof(HEADER), __FILE__, __LINE__);
	if(nullptr == pBuf)
	{
		return false;
	}
	//
	if(!pBuf->Append(bufZip.GetBufPtr(), (int)sizeof(HEADER) + nCompressLen))
	{
		LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
		return false;
	}
	PHEADER pHeader = (PHEADER) bufZip.GetBufPtr();
	pHeader->dwLength = nCompressLen;
	pHeader->dwType |= CMP_MSG;
	//
	SendMsg(pBuf);
	return true;
}

bool CNetClient::SendMsg(CBuffer* pBuf)
{
	if(pBuf == nullptr)
	{
		return false;
	}
	CAutoLock locker(&m_clsSend);
	m_listSendMsg.push(pBuf);
	UpdateEventType(EPOLLOUT | EPOLLET);
	return true;
}

void CNetClient::BindUserObj(CUserObject* pUser)
{
	m_pUserObj = pUser;

	if(!IsBindUserObject())
		return ;
	//m_pUserObj->BindNetClient(this);
	//m_pUserObj->SetPeerAddr(m_dwIP, m_nPort);
}

inline bool CNetClient::IsBindUserObject()
{
	if(nullptr == m_pUserObj)
		return false;
	return true;
}

inline void CNetClient::SetOrigin(int wOrigin)
{
	m_wOrigin = wOrigin;
}

void CNetClient::ProcessMsg()
{
	CBuffer* pBuf = nullptr;
	while(true)
	{
		if(m_listRecvMsg.empty())
			break;

		//开始消费消息	
		{
			CAutoLock locker(&m_clsRecv);
			pBuf = m_listRecvMsg.front();
			m_listRecvMsg.pop();
		}
		PHEADER pMsg = (PHEADER)pBuf->GetBufPtr();
		//压缩消息解压
		if(pMsg->dwType & CMP_MSG)
		{
			CBuffer bufUnzip;
			bufUnzip.ExpandTo(pMsg->dwLength * 10 + sizeof(HEADER));
			if(!bufUnzip.Append(pMsg, sizeof(HEADER)))
			{
				LogError("%s( %d) Apend Data fail.",__FILE__, __LINE__);
				g_pBufferMgr->ReleaseBuffer(pBuf);
				return ;
			}
			int nUnCompressLen = 0;
			if(!UnCompress(bufUnzip.GetBufPtr() + sizeof(HEADER), nUnCompressLen, (char*)(pMsg + 1), pMsg->dwLength))
			{
				LogError("%s(%d) UnCompressed Message fail.", __FILE__, __LINE__);
				g_pBufferMgr->ReleaseBuffer(pBuf);
				return ;
			}
			pBuf->Exchange(bufUnzip);
			PHEADER pHeader = (PHEADER) pBuf->GetBufPtr();
			if(nullptr == pHeader)
			{
				LogError("%s(%d) pHeader is NULL.", __FILE__, __LINE__);
				g_pBufferMgr->ReleaseBuffer(pBuf);
				return ;
			}
			pHeader->dwType &= ~CMP_MSG;
			pHeader->dwLength = nUnCompressLen;
		}
		//业务层处理消息
		OnMsg(pMsg);
		g_pBufferMgr->ReleaseBuffer(pBuf);	
	}
}

// /*******************************************************
/**
 * @brief: UpdateEventType 
 *
 * @param nType
 */
// *******************************************************/
void CNetClient::UpdateEventType(int nType)
{
	if(m_nEvent & nType)
		return ;
	
	m_nNewEvent = nType;
	
	m_pNetSercie->AddNetClient(this);
}

//服务端主动断开
bool CNetClient::Terminate()
{
	Close();
	OnBreak();	
	return true;
}
