#include "ActiveIO.h"
#include "LogFile.h"
#include "GFunc.h"
#include <assert.h> 

CActiveIO::CActiveIO():m_tBreak(0), m_tLastestMsg(0)
{
	m_fd = -1;
	m_pOwner = nullptr;
}

CActiveIO::~CActiveIO()
{
	Terminate();
}


void CActiveIO::OnBreak()					//断开调用
{
	
}

void CActiveIO::Terminate()				//终止
{
	if(m_pOwner)
	{
		m_pOwner->OnClose();
	}
}


bool CActiveIO::OnRecv()
{
	
	return m_pOwner->OnRecv();
}

bool CActiveIO::OnSend()
{
	return m_pOwner->OnSend();
}

bool CActiveIO::OnClose()
{
	OnBreak();
	m_nNewEventType = EPOLL_EVENT_TYPE_CLOSE;
	return true;
}
	

bool CActiveIO::SendMsg(PHEADER pHeader)		//Sync Send 
{
	std::unique_lock<std::mutex>	locker(m_clsSendLock);
	if(nullptr == pHeader)
		return false;

	unsigned int dwDataLen = pHeader->dwLength + sizeof(HEADER);
	pHeader->dwLength = htonl(pHeader->dwLength);		//
	unsigned int dwSended = 0;
	while(true)
	{
		int nSend = m_pOwner->Send((char*)pHeader + dwSended,  dwDataLen - dwSended);
		if(nSend <= 0)
		{
			if(EINTR == errno || EAGAIN == errno)
				continue;
			else 
			{
				char szIP[16] = { 0 };
				LogError("SendMsg fail. error:%d, Dst IP:Port = %s:%d", errno, HostIP2Str(m_dwHostIP, szIP), m_nPort);
				Terminate();
				return false;
			}
		}
		dwSended += nSend;
		if(dwSended >= dwDataLen)
			return true;
	}

	return true;
}

bool CActiveIO::PeerMsg(Buffer* pBuf)
{
	// 同步操作 
	int nMode =  fcntl(m_fd, F_GETFL);
	if(nMode & O_NONBLOCK)
	{
		return false;
	}
//
	fcntl(m_fd, F_SETFL, nMode | fcntl(m_fd, F_GETFL));
//
	unsigned int  dwRecved = 0;
	unsigned int dwDataLen = sizeof(HEADER);
	unsigned int dwRecv = sizeof(HEADER);
	int cnRecv = 5;
	while(cnRecv--)
	{
		if(pBuf->size() < dwDataLen)
			pBuf->resize(dwDataLen);

		int nRecv = m_pOwner->Recv(&pBuf->at(0) + dwRecved, dwRecv);
		if(nRecv <= 0)
		{
			if(EINTR == errno || EAGAIN == errno)
				continue;
			else 
			{
				char szIP[16] = { 0 };
				LogError("Recv Peer Msg fail. error:%d, Dst IP:Port = %s:%d", errno, HostIP2Str(m_dwHostIP, szIP), m_nPort);
				return false;
			}
		}
		if(dwDataLen == (unsigned int)nRecv && dwRecv == sizeof(HEADER))
		{
			PHEADER pMsg = (PHEADER)&pBuf->at(0);
			dwDataLen = pMsg->dwLength + sizeof(HEADER);
		}
		dwRecved += nRecv;
		if(dwRecved >= dwDataLen)
		{
			break;
		}
	}
	//
	fcntl(m_fd, F_SETFL,  ~O_NONBLOCK & fcntl(m_fd, F_GETFL));
	return true;
}

void CActiveIO::PermitRead()
{
	m_nNewEventType = EPOLLIN;
}

bool CActiveIO::IsReadable()
{
	return m_nEventType & EPOLLIN;
}

void CActiveIO::ForbidWrite()
{
	m_nNewEventType = m_nEventType & ~EPOLLOUT;
}
	
bool CActiveIO::OnMsg(PHEADER& pHeader)			//消息到达接口
{
	return true;
}
	
bool CActiveIO::OnInit(CNetIO* pNetIO)
{
	if(nullptr == pNetIO)
		return false;

	m_pOwner = pNetIO;
	m_fd = pNetIO->Detach();
	pNetIO->SetOwner((CNetIO*)this);
	PermitRead();

	return true;
}
	
