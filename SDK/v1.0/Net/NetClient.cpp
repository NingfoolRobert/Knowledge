#include "NetClient.h"
#include "AutoLock.h"
#include "BufferMgr.h"
#include "LogFile.h"
#include "UserObject.h"
#include "NetService.h"


CNetClient::CNetClient():m_pUserObject(nullptr),m_pNetService(nullptr),
						m_pSendBuf(nullptr), m_dwSendLen(0),
						m_pRecvBuf(nullptr), m_dwRecvLen(0),
						m_dwSendSerialNum(0), m_dwRecvSerialNum(0),m_dwOrigin(0)
{
	PermitRead();
}

CNetClient::~CNetClient()
{
	if(nullptr == m_pUserObject)
	{
		m_pUserObject->BindNetClient(nullptr);
		DEC_REF(m_pUserObject);
		m_pUserObject = nullptr;
	}
}

bool  CNetClient::Init(int& fd, CNetService* pNetService)
{
    Attach(fd);
    if(nullptr == pNetService)
    {
        return false;
    }
    m_pNetService = pNetService;
    struct sockaddr_in PeerAddr;
    socklen_t nLen = sizeof(PeerAddr);
    if(!CSocket::GetPeerName((struct sockaddr*)&PeerAddr,&nLen))
    {
        LogError("Acquire PeerAddr fail.");
        return false;
    }

    m_nPort = ntohs(PeerAddr.sin_port);
    m_dwIP = ntohl(PeerAddr.sin_addr.s_addr);

	SetBlockMode();
    return true;
}

//bool CNetClient::OnMsg(PHEADER pHeader)
//{
//	if(nullptr == pHeader || nullptr == m_pNetService)
//		return false;
//
//	if(!IsBindUserObject())
//	{
//		return m_pNetService->OnNetMsg(this, pHeader);
//	}
//
//	//校验消息头
//	if(pHeader->wOrigin != m_dwOrigin)
//	{
//		// 强制关闭 socket  
//	}
//	//
//	return m_pUserObject->OnMsg(pHeader);
//}    

void CNetClient::OnConnect()
{
    if(IsBindUserObject())
    {
		m_pUserObject->m_tConnection = time(NULL);
        m_pUserObject->OnConnect();
    }
    //
    LogError("Not Bind UserObject.");
}

void CNetClient::OnTickCnt();
{
    if(IsBindUserObject())
    {
        m_pUserObject->OnTickCnt();
    }
}

void CNetClient::OnBreak()
{
    if(IsBindUserObject())
    {
		m_pUserObject->m_tBreak = time(NULL);
        m_pUserObject->OnClose();
		m_pUserObject->BindNetClient(nullptr);
    }
    //
    LogError("Not Bind UserObject.");
}

bool CNetClient::SendMsg(PHEADER pMsg)
{
    if(nullptr == pMsg)
    {
        return false;
	}

	if(m_nNewEventType == EPOLL_EVENT_TYPE_CLOSE || m_nEventType == EPOLL_EVENT_TYPE_CLOSE)
		return false;

    return SendMsg((char*)pMsg, pMsg->dwLength + sizeof(HEADER));
}


bool CNetClient::SendMsg(CBuffer* pBuffer)
{
    if(nullptr == pBuffer || pBuffer->GetBufLen() < (int)sizeof(HEADER))
        return false;
    
    return SendMsg(pBuffer->GetBufPtr(), pBuffer->GetBufLen());
}

void CNetClient::BindUserObject(CUserObject* pUserObj)
{
    m_pUserObject = pUserObj;
}
    
bool CNetClient::IsBindUserObject()
{
    return m_pUserObject != nullptr;
}

bool CNetClient::OnSend()
{
    int nSendLen = 0;
    while(true)
    {
        if(nullptr == m_pSendBuf)
        {
            CAutoLock locker(&m_clsSendLock);
            if(!m_listSendBuf.empty())
            {
                m_pSendBuf = m_listSendBuf.front();
                m_listSendBuf.pop();
            }
        }
        //
        if(nullptr == m_pSendBuf)
            return true;
        //
        nSendLen =  CSocket::Send(m_pSendBuf->GetBufPtr(), m_pSendBuf->GetBufLen() - m_dwSendLen);
        if(nSendLen == m_pSendBuf->GetBufLen() - (int)m_dwSendLen)
        {
            g_pBufferMgr->ReleaseBuffer(m_pSendBuf);
            m_pSendBuf = nullptr;
            m_dwSendLen = 0;
        }
        else if(nSendLen == 0)
        {
            //CounterParter Close
            char szIP[16] = { 0 };
            HostIP2Str(m_dwIP, szIP);
            LogTrace("Socket closed. IP:Port=%s:%d", szIP, m_nPort);
			Terminate();   
			return false;
        }
        else if (nSendLen < 0)
        {
            //Send Error;
            if(EAGAIN == errno)
            {
                break;      //发送区已满
            }
            else if(EINTR == errno)
            {
                continue;     
            }
            else 
            {
				char szIP[32] = { 0 };
				HostIP2Str(m_dwIP, szIP);
				LogError("Send message error: %d.", errno);
            }
        }
        else 
        {
            m_dwSendLen += nSendLen;
            break;
        }
        
    }
    return true;
}
    
bool CNetClient::OnRecv()
{
    if(m_pRecvBuf == nullptr)
    {
        m_pRecvBuf = new CBuffer;
        if(nullptr == m_pRecvBuf)
        {
            LogError("Alloc memmory fail.");
            return false;
        }
    }
    //Recv data 
    char szBuf[1024] = { 0 };
	int bTerminate = true;
	while(
    {
        int nRecvLen = CSocket::Recv(szBuf, 1024);
        if(nRecvLen < 0)
        {
            if(EAGAIN == errno)
            {
                break;      //接收区无数据可读
            }
            else if(EINTR ==  errno)    //进程调试 控制台切换 子进程退出 任务切换等
            {
                continue;
            }
            else 
            {
                char szIP[32] = { 0 };
                HostIP2Str(m_dwIP, szIP);
                LogError("Recv Message error! IP:Port = %s:%d", szIP, m_nPort);
                break;
            }
        }
        else if(nRecvLen == 0)
        {
			if(!bTerminate)
				break;
			char szIP[16] = { 0 };
            HostIP2Str(m_dwIP, szIP);
            LogTrace("CounterPart Close. IP:Port=%s:%d", szIP, m_nPort);
			Terminate();
			return false;
		}
        else 
        {
			bTerminate = false;
            if(!m_pRecvBuf->Append(szBuf, nRecvLen))
            {
                LogError("Append Data fail.");
                break;
            }
        }    
    }
    //
    int nMsgLen = 0;
    while(true)
    {
        if(m_pRecvBuf->GetBufLen() - m_dwRecvLen < HEADSIZE)
            break;
        PHEADER pHeader = (PHEADER)(m_pRecvBuf->GetBufPtr() + m_dwRecvLen);
        if(nullptr == pHeader)
            return true;
        //
        nMsgLen = pHeader->dwLength + HEADSIZE;
        if(nMsgLen > m_pRecvBuf->GetBufLen() - (int)m_dwRecvLen)
            break;
        //
        auto pBuffer = g_pBufferMgr->GetBuffer(nMsgLen);
        if(nullptr == pBuffer)
        {
            LogError("alloc Buffer fail.");
            return false;
        }
        //
        if(!pBuffer->Append(pHeader, nMsgLen))
        {
            LogError("Append Data fail.");
            return false;
        }
        m_dwRecvLen += nMsgLen;
        {
            CAutoLock locker(&m_clsRecvLock);
            m_listRecvBuf.push(pBuffer);
        }
		//
		if(m_dwRecvLen == (unsigned int)m_pSendBuf->GetBufLen())
		{
			if(m_pSendBuf->GetBufLen() >= 1024 * 1024)
				m_pSendBuf->Clear(true);
			else  
				m_pSendBuf->Clear(false);
			m_dwRecvLen = 0;
		}
	}
	//
	if(m_listRecvBuf.size())
	{
		m_pNetService->OnNetMsg(this);
	}
    return true;
}

void CNetClient::ActiveRecvThread()
{
    CBuffer* pBuf = nullptr;
    while(true)
    {
        m_clsRecvLock.Lock();
        if(m_listRecvBuf.empty())
        {
            m_clsRecvLock.UnLock();
            break; 
        }   
        pBuf = m_listRecvBuf.front();
        m_listRecvBuf.pop();
        m_clsRecvLock.UnLock();
        
        if(nullptr != pBuf)
        {
            PHEADER pMsg = (PHEADER)pBuf->GetBufPtr();
            OnMsg(pMsg);
            g_pBufferMgr->ReleaseBuffer(pBuf);
        }
    }

	if(m_nEventType == EPOLL_EVENT_TYPE_CLOSE && m_listRecvBuf.empty())
	{
		m_pNetService->DelClient(this);
	}
}

//Direct Send Peer
bool CNetClient::SendMsg(const char* pBuf, int nLen)
{
    if(nullptr == pBuf || 0 == nLen)
    {
        return true;
    }

	int nSended = 0;
	while(nLen > 0)
	{
		int nSend = Send(pBuf + nSended, nLen);
		if(nSend == 0)
		{
			char szIP[16] = { 0 };
			HostIP2Str(m_dwIP, szIP)；
			LogError("Connection break,IP:Port=%s:%d", szIP, m_nPort);	
			return false;
		}
		
		nSended += nSend;
		nLen -= nSend;
	}
    //
   // CAutoLock locker(&m_clsSendLock);
   // //Current Async Send 
   // if(nullptr != m_pSendBuf || !m_listSendBuf.empty())
   // {
   //     CBuffer* pBuffer = g_pBufferMgr->GetBuffer(nLen);
   //     if(nullptr == pBuffer)
   //     {
   //         LogError("Alloc new Buffer fail.");
   //         return false;
   //     }
   //     //
   //     if(!pBuffer->Append(pBuf, nLen))
   //     {
   //         LogError("Append Data fail.");
   //         return false;
   //     }
   //     
   //     m_listSendBuf.push(pBuffer);
   //     return true;
   // }
   // 
   // int nSended = CSocket::Send(pBuf, nLen);
   // if(nSended == nLen)
   // {
   //     return true;
   // }
   // else if(nSended > 0)
   // {
   //     CBuffer* pBuffer = g_pBufferMgr->GetBuffer(nLen);
   //     if(nullptr == pBuffer)
   //     {
   //         LogError("Alloc new Buffer fail.");
   //         return false;
   //     }
   //     //
   //     if(!pBuffer->Append(pBuf + nSended, nLen - nSended))
   //     {
   //         LogError("Append Data fail.");
   //         return false;
   //     }
   //     m_pSendBuf = pBuffer;
   //     return true;
   // }
   // else if(nSended == 0)
   // {
   //     char szIP[32] = { 0 };
   //     HostIP2Str(m_dwIP, szIP);
   //     LogError("Peer Socket closed, IP:Port=%s:%d", szIP, m_nPort);
   //     return false;
   // }
   // else 
   // {
   //     if(EAGAIN == errno || EINTR == errno)
   //     {
   //         CBuffer* pBuffer = g_pBufferMgr->GetBuffer(nLen);
   //         if(nullptr == pBuffer)
   //         {
   //             LogError("Alloc new Buffer fail.");
   //             return false;
   //         }
   //         //
   //         if(!pBuffer->Append(pBuf, nLen))
   //         {
   //             LogError("Append Data fail.");
   //             return false;
   //         }
   //         
   //         m_listSendBuf.push(pBuffer);
   //     }
   //     else 
   //     {
   //         char szIP[32] = { 0 };
   //         HostIP2Str(m_dwIP, szIP);
   //         LogError("Peer Socket Send errno:%d, IP:Port=%s:%d", errno, szIP, m_nPort);
   //         return false;
   //     }
   // }
	return true;
}

void CNetClient::Terminate()
{
	m_nNewEventType = EPOLL_EVENT_TYPE_CLOSE;
	if(m_pNetService)
    {
        m_pNetService->OnNetBreak(this);
    }
	OnBreak();
}

bool CNetClient::OnClose()
{
	if(m_pNetService)
	{
		shutdown(m_fd, SHUT_WR);
		if(m_nEventType == EPOLL_EVENT_TYPE_CLOSE && m_listRecvBuf.empty())
			m_pNetService->DelClient(this);
		OnBreak();
	}
	return true;
}
	
//bool CNetClient::ProcessMsg()
//{
//	CBuffer* pBuf = nullptr;
//	PHEADER pMsg = nullptr;
//	CAutoLock locker(&m_clsRecvLock);
//	// TODO 
//	if(nullptr == m_pRecvBuf)
//		return false;
//	//
//	int nOffset = 0;
//	int nDataLen = m_pRecvBuf->GetBufLen();
//	while(nDataLen > sizeof(HEADER))
//	{
//		if(m_dwRecvLen == 0 )
//		{
//			pMsg = (PHEADER)pBuf->GetBufPtr();
//			m_dwRecvLen = pMsg->dwLength + sizeof(HEADER);
//		}
//		//
//		if(m_dwRecvLen > nDataLen)
//			break;
//		//
//		pMsg = (PHEADER)m_pRecvBuf->GetBufPtr();
//		if(nullptr == pMsg)
//		{
//			return false;
//		}
//		//TODO 
//			
//		pBuf = g_pBufferMgr->GetBuffer(m_pRecvBuf->GetBufLen() - m_dwRecvLen);
//		if(nullptr == pBuf)
//		{
//			LogError("memory error");
//			return false;
//		}
//		//
//		m_pUserObject->OnMsg(pMsg);
//	}
//
//	g_pBufferMgr->ReclaimBuffer(pBuf);
//	return true;
//}
	
CUserObject* CNetClient::GetUserObject()
{
	return m_pUserObject;
}
	
	
bool CNetClient::ProcessMsg()
{
	CBuffer* pBuffer = nullptr;
	PHEADER pMsg = nullptr;
	while(!m_listRecvBuffer.IsEmpty())
	{
		m_clsRecvLock.Lock();
		pBuffer = m_listRecvBuffer.Get();
		m_clsRecvLock.UnLock();
		//
		if(nullptr == pBuffer)
			continue;
		pMsg = (PHEADER)pBuffer->GetBufPtr();
		if(nullptr == pMsg)
		{
			delete pMsg;
			continue;
		}	
		m_pNetService->OnNetMsg(this,pMsg);	
//		m_pUserObject->OnMsg(pMsg);
		g_pBufferMgr->ReclaimBuffer(pBuffer);
	}
}
	
bool CNetClient::SendMsg(PHEADER pMsg, bool bAsync/* = true*/)
{
	if(nullptr == pMsg)
		return false;
	
	if(bAsync)
	{
		CBuffer* pBuf = g_pBufferMgr->GetBuffer(pMsg->dwLength + sizeof(HEADER));
		if(nullptr == pBuf)
		{
			LogError("memory error.");
			return false;
		}

		if(!pBuf->Append(pMsg, pMsg->dwLength + sizeof(HEADER)))
		{
			LogError("Append Data fail.");
			return false;
		}
		
		CAutoLock locker(&m_clsSendLock);
		m_listASendBuffer.Add(pBuf);	
		PermitWrite();
		return true;
	}

	CAutoLock locker(&m_clsSendLock);
	return SendMsg((char*)pMsg, pMsg->dwLength + sizeof(HEADER));
}

bool CNetClient::SendMsg(CBuffer* pBuffer, bool bAsync/* = true*/)
{
	if(nullptr == pBuffer)
		return false;
	//异步队列
	if(bAsync)
	{
		CBuffer* pBuf = g_pBufferMgr->GetBuffer(pBuffer->GetBufLen());
		if(nullptr == pBuf)
		{
			LogError("memory error.");
			return false;
		}

		pBuf->Exchange(pBuffer);
	
		CAutoLock locker(&m_clsSendLock);
		m_listASendBuffer.Add(pBuf);
		PermitWrite();
		return true;

	}

	//
	CAutoLock locker(&m_clsSendLock);
	return SendMsg(pBuffer->GetBufLen(), pBuffer->GetBufLen());
}


bool CNetClient::EncrptMsg(constr char* pszSrc, unsigned int nSrcLen, CBuffer* pBuffer)
{
	return true;
}

bool CNetClient::UnEncrptMsg(const char* pszSrc, unsigned int nSrcLen, CBuffer* pBuffer)
{
	return true;
}
