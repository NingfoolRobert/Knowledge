#include "NetClient.h"
#include "UserObject.h"

CNetClient::CNetClient()
{

}

CNetClient::~CNetClient()
{

}


bool CNetClient::SendMsg(PHEADER pMsg)
{
	if(nullptr == pMsg)
		return false;
	//TODO
	return true;
}

void CNetClient::Terminate(bool bWhenSendOver/* = false*/)
{

}

bool CNetClient::OnMsg(PHEADER pMsg)
{
	if(nullptr == m_pUserObject)
		return false;
	m_pUserObject->OnMsg(pMsg);
	return true;
}

bool CNetClient::OnConnect()
{
	if(nullptr == m_pUserObject)
		return false;
	m_pUserObject->OnConnect();
	return true;
}
bool CNetClient::OnBreak()
{
	if(nullptr == m_pUserObject)
		return false;
	m_pUserObject->OnBreak();
	return true;
}
bool CNetClient::OnTickCount()
{
	if(nullptr == m_pUserObject)
		return false;
	m_pUserObject->OnTickCount();
	return true;
}

inline void CNetClient::SetMsgOrigine(unsigned short usMsgOrigine)
{
	m_uMsgOrigin = usMsgOrigine;
}

void CNetClient::SetRecvIdleTime(int nSecond)
{
	
}

inline void CNetClient::BindUserObject(CUserObject* pUser)
{
	if(nullptr == pUser)
		return ;
	m_pUserObject = pUser;
}//TODO

inline bool CNetClient::IsBindUserObject()
{
	if(nullptr == m_pUserObject)
		return false;
	return true;
}

bool CNetClient::OnSend()
{
	if(nullptr == m_pSendBuf)
	{
		bool bRet  = m_listSend.Dequeue(m_pSendBuf);
		if(!bRet)
			return false;
	}
	
	int nSend = Send(m_pSendBuf->GetBufPtr(), m_pSendBuf->GetBufLen());
	
	CBuffer * pSendBuf = nullptr;
	if(nSend != m_pSendBuf->GetBufLen())
	{

		pSendBuf = new CBuffer;
		if(pSendBuf == nullptr)
		{
			//TODO 
			pSendBuf->Append(m_pSendBuf->GetBufPtr() + nSend, m_pSendBuf->GetBufLen() - nSend);
			m_pSendBuf = pSendBuf;
			return false;
		}
		
	}
	
	
	

	return true;
}
bool CNetClient::OnReceive()
{
	if(m_pRecvBuf == nullptr)
	{
		//TODO Recvice Message
	}
	
	int nLength  = 0;
	PHEADER pHeader = (PHEADER)m_pRecvBuf->GetBufPtr();
	
	if(pHeader != nullptr)
	{
		if(pHeader->uOrigin == m_uMsgOrigin && pHeader->uSerial == m_uLastRecvSerial + 1)
		{
			if(m_pRecvBuf->GetBufLen() == (int)pHeader->uLength + sizeof(HEADER))
			{
				m_listRecv.Enqueue(m_pRecvBuf);
				m_pRecvBuf = nullptr;
			}
			else 
			{
				//TODO 
				CBuffer* pBuffer = new CBuffer;
				if(nullptr == pBuffer)
				{
					//TODO 
					nLength = pHeader->uLength + sizeof(HEADER);
					pBuffer->Append(m_pRecvBuf->GetBufPtr() + nLength, m_pRecvBuf->GetBufLen() - nLength);
					m_listRecv.Enqueue(m_pRecvBuf);
					m_pRecvBuf = pBuffer;
					m_uLastRecvSerial += 1;
				}
			}
			
		}
		else 
		{
			int nLen = 0;
			CBuffer* pBuffer = new CBuffer;
			if(nullptr == pBuffer)
			{
				//TODO 
				 return false;
			}
			while(1)
			{
				if(m_pRecvBuf->GetBufLen() == 0)
					break;
				PHEADER pHead = m_pRecvBuf->GetBufPtr() + nLen;
				if(pHead != nullptr)
				{
					
				}
			}
		}
		
	}

	return true;
}
bool CNetClient::OnActiveMsg()
{
	return true;
}
bool CNetClient::MessageTrans(CBuffer* pBuffer, int& len)
{
	len = 0;
	if(pBuffer == nullptr)
		return false;
	PHEADER pHeader = (PHEADER)pBuffer->GetBufPtr();
	if(pHeader == nullptr)
	{
		//TODO 
		return false;
	}
	if(pBuffer->GetBufLen() >=(int)( pHeader->uLength + sizeof(HEADER)))
	{
		if(pHeader-> uSerial == m_uLastRecvSerial + 1 && pHeader->uOrigin == m_uMsgOrigin)
		{
			CBuffer* pBuffer = new CBuffer;
			if(pBuffer == nullptr)
			{
				//TODO 
				return false;
			}
			m_listRecv.Enqueue(m_pRecvBuf);
		}
		else 
		{
			int nlen = 0;
			if(pBuffer->GetBufLen() < sizeof(HEADER))
				return true;
			while(true)
			{
				if(pBuffer->GetBufLen()  < nlen + sizeof(HEADER))
					return true;
				PHEADER pHead  = (PHEADER)pBuffer->GetBufPtr() + nlen;
				if(pHead != nullptr)
				{
					//TODO 
					return false;
				}
				if(pHead->uOrigin != m_uMsgOrigin || pHead->uSerial != m_uLastRecvSerial + 1)
				{
					//break;
					nlen++;
					continue;
				}
				else 
				{
					CBuffer* pBuffer =  new CBuffer;
					if(pBuffer == nullptr)
					{
						return false;
					}
					pBuffer->Append(m_pRecvBuf->GetBufPtr() + nlen,pHead->uLength);
					nlen +=pHead->uLength;
				}

				//if(pBuffer->GetBufLen() < pHe)
			}
		}
	}


	return true;
}
