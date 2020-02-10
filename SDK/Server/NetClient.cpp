#include "NetClient.h"
#include "AutoLock.h"
#include "BufferMgr.h"
#include "LogFile.h"
#include "UserObject.h"


CNetClient::CNetClient(void):m_pUserObject(nullptr),m_pSendBuf(nullptr),
                            m_pRecvBuf(nullptr), m_ulRecvSerialNum(0), 
                            m_ulSendSerialNum(0), m_dwSendLen(0), m_dwRecvLen(0)
{
}

CNetClient::~CNetClient()
{
}
bool  CNetClient::Init(int& fd, CNetService* pNetService)
{
    CSocket::Attach(fd);
    if(nullptr == pNetService)
    {
        return false;
    }
    
    struct sockaddr_in PeerAddr;
    socklen_t nLen = sizeof(PeerAddr);
    if(!CSocket::GetPeerName((struct sockaddr*)&PeerAddr,&nLen))
    {
        LogError("%s(%d)  Acquire PeerAddr fail.", __FILE__, __LINE__);
        return false;
    }

    m_nPort = ntohs(PeerAddr.sin_port);
    m_dwIP = ntohl(PeerAddr.sin_addr.s_addr);
    return true;
}

bool CNetClient::OnMsg(PHEADER pHeader)
{
    //init UserObject 
    if(m_ulRecvSerialNum == 0)
    {
        if(nullptr == m_pNetService)
        {
            return false;
        }
        //init UserObject;
        CUserObject* pObj = m_pNetService->OnNetUserObject(pHeader);
        if(nullptr == pObj)
        {
            LogError("%s(%d) Alloc UserObject fail.", __FILE__, __LINE__);
            return false;
        }
        pObj->SetPeerAddr(m_dwIP, m_nPort);
        pObj->BindNetClient(this);

        BindUserObject(pObj);
    }
    //发送序列号
    m_ulRecvSerialNum++;
    //
    if(IsBindUserObject())
    {
        return m_pUserObject->OnMsg(pHeader);
    }

    //
    LogError("%s(%d) Not Bind UserObject.", __FILE__, __LINE__);
    return false;
}    

void CNetClient::OnConnect()
{
    if(IsBindUserObject())
    {
        m_pUserObject->OnConnect();
    }
    //
    LogError("%s(%d) Not Bind UserObject.", __FILE__, __LINE__);
}

bool CNetClient::OnSecondIdle()
{
    if(IsBindUserObject())
    {
        m_pUserObject->OnSecondIdle();
        return true;
    }
    return false;
}

void CNetClient::OnBreak()
{
    if(IsBindUserObject())
    {
        m_pUserObject->OnClose();
        m_pUserObject->BindNetClient(nullptr);
        DEC_REF(m_pUserObject);
        m_pUserObject = nullptr;
    }
    
    //
    LogError("%s(%d) Not Bind UserObject.", __FILE__, __LINE__);
}

bool CNetClient::SendMsg(PHEADER pMsg)
{
    if(nullptr == pMsg)
    {
        return false;
    }
    return SendMsg((void*)pMsg, pMsg->dwLength + sizeof(HEADER));
}

bool CNetClient::SendZipMsg(PHEADER pMsg)
{
    //TODO zip message 
    
    auto pBuf  = g_pBufferMgr->GetBuffer(pMsg->dwLength + HEADSIZE);
    if(nullptr == pBuf)
    {
        LogError("%s(%d) Alloc memmory fail");
        return false;
    }
    //
    //if(!pBuf->Append(pMsg, pMsg->dwLength + HEADSIZE))
    //{
    //   LogError("%s(%d) Append Data fail.");
    //    return false;
    //
    //TODO zip message;
    //TODO append zip message;
    return SendMsg(pBuf);
}

bool CNetClient::SendMsg(CBuffer* pBuffer)
{
    if(nullptr == pBuffer || pBuffer->GetBufLen() < sizeof(HEADER))
        return false;
    
    //auto pBuf = g_pBufferMgr->GetBuffer(pBuffer->GetBufLen());
    //if(nullptr == pBuf)
    //{
    //    LogError("%s(%d) Alloc CBuffer fail.", __FILE__, __LINE__);
    //    return false;
    //}
    //
   // pBuf->Exchange(pBuffer);
    //First direct send 
    
    //
    //CAutoLock locker(&m_clsSendLock);
    //m_listSendBuf.push(pBuffer);
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
        nSendLen =  CSocket::Send(m_pSendBuf->GetBufPtr(), m_pSendBufLen());
        if(nSendLen == m_pSendBuf->GetBufLen() - m_dwSendLen)
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
                char szIP[32] =　{ 0 };
                HostIP2Str(m_dwIP, szIP);
                LogError("%s(%d) Send message error: %d.", errno);
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
            LogError("%s(%d) Alloc memmory fail."  __FILE__, __LINE__);
            return false;
        }
    }
    //Recv data 
    char szBuf[1024] = { 0 };
    while(true)
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
            char szIP[16] = { 0 };
            HostIP2Str(m_dwIP, szIP);
            LogTrace("CounterPart Close. IP:Port=%s:%d", szIP, m_nPort);
            UpdateEvent(EPOLLCOLSE);
        }
        else 
        {
            if(!m_pRecvBuf->Append(szBuf, 1024))
            {
                LogError("%s(%d) Append Data fail.");
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
        if(nMsgLen > m_pRecvBuf->GetBufLen() - m_dwRecvLen)
            break;
        //
        auto pBuffer = g_pBufferMgr->GetBuffer(nMsgLen);
        if(nullptr == pBuffer)
        {
            LogError("%s(%d) alloc Buffer fail.");
            return false;
        }
        //
        if(!pBuffer->Append(pHeader, nMsgLen))
        {
            LogError("%s(%d) Append Data fail.");
            return false;
        }
        m_dwRecvLen += nMsgLen;
        {
            CAutoLock locker(&m_clsRecvLock);
            m_listRecvBuf.push(pBuffer);
        }
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
}

//Direct Send Peer
bool CNetClient::SendMsg(void* pBuf, int nLen)
{
    if(nullptr == pBuf || 0 == nLen)
    {
        return true;
    }
    //
    CAutoLock locker(&m_clsSendLock);
    //Current Async Send 
    if(nullptr != m_pSendBuf || !m_listSendBuf.empty())
    {
        CBuffer* pBuffer = g_pBufferMgr->GetBuffer(nLen);
        if(nullptr == pBuffer)
        {
            LogError("%s(%d) Alloc new Buffer fail.", __FILE__, __LINE__);
            return false;
        }
        //
        if(!pBuffer->Append(pBuf, nLen))
        {
            LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
            return false;
        }
        
        m_listSendBuf.push(pBuffer);
        return true;
    }
    
    int nSended = CSocket::Send(pBuf, nLen);
    if(nSended == nLen)
    {
        return true;
    }
    else if(nSended > 0)
    {
        CBuffer* pBuffer = g_pBufferMgr->GetBuffer(nLen);
        if(nullptr == pBuffer)
        {
            LogError("%s(%d) Alloc new Buffer fail.", __FILE__, __LINE__);
            return false;
        }
        //
        if(!pBuffer->Append(pBuf + nSended, nLen - nSended))
        {
            LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
            return false;
        }
        m_pSendBuf = pBuffer;
        return true;
    }
    else if(nSended == 0)
    {
        char szIP[32] = { 0 };
        HostIP2Str(m_dwIP, szIP);
        LogError("Peer Socket closed, IP:Port=%s:%d", szIP, m_nPort);
        return false;
    }
    else 
    {
        if(EAGAIN == errno || EINTR == errno)
        {
            CBuffer* pBuffer = g_pBufferMgr->GetBuffer(nLen);
            if(nullptr == pBuffer)
            {
                LogError("%s(%d) Alloc new Buffer fail.", __FILE__, __LINE__);
                return false;
            }
            //
            if(!pBuffer->Append(pBuf, nLen))
            {
                LogError("%s(%d) Append Data fail.", __FILE__, __LINE__);
                return false;
            }
            
            m_listSendBuf.push(pBuffer);
            PermitSend();
        }
        else 
        {
            char szIP[32] = { 0 };
            HostIP2Str(m_dwIP, szIP);
            LogError("Peer Socket Send errno:%d, IP:Port=%s:%d", errno, szIP, m_nPort);
            return false;
        }
    }
}

bool CNetClient::Terminate()
{
    if(m_pNetService == nullptr)
    {
        return m_pNetService->DelClient(this);
    }
    return true;
}