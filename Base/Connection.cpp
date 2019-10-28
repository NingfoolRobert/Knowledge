#include "Connection.h"
#include "Log.h"
#include "GFunc.h"

#include <sys/select.h>
#include <sys/time.h>
#include <vector>



CConnection::CConnection()
{
	m_pHeader =  nullptr;
}

CConnection::~CConnection()
{

}

bool CConnection::SendMsg(unsigned int dwHostIP, int nPort, PHEADER pMsg, bool bWaitRecv/* = false*/)
{
	if(nullptr == pMsg)
	{
		LogError("%s(%d) pMsg is NULL.", __FILE__, __LINE__);
		return false;
	}

	char szTmp[32] = { 0 };
	IPHost2Str(szTmp, dwHostIP);
	if(Create() <= 0)
	{
		LogError("%s(%d) Create Socket fail. IP:Port = %s:%d", __FILE__, __LINE__, szTmp, m_nPort);
		return false;
	}

	if(!Connect(dwHostIP, nPort))
	{
		LogError("%s(%d) Create Socket fail. IP:Port = %s:%d", __FILE__, __LINE__, szTmp, m_nPort);
		return false;
	}
	
	if(Send((char*)pMsg, pMsg->dwLength + sizeof(HEADER)) != pMsg->dwLength + sizeof(HEADER))
	{
		LogError("%s(%d) Create Socket fail. IP:Port = %s:%d", __FILE__, __LINE__, szTmp, m_nPort);
		return false;
	}
	//设置为非阻塞模式定时接受数据 
	SetBlockMode();

	fd_set rset;
	
	FD_ZERO(&rset);

	m_pbufRecv->Clear();
	struct timeval tm;
	tm.tv_sec = 0;
	tm.tv_usec = 10000;
	for(int i = 0; i < 100; i ++)
	{
		int nRet = select(Detach()+1, &rset, NULL,NULL, &tm);
		if(nRet < 0)
		{
			LogError("");
			return false;
		}
		else if(nRet == 0)
		{
			continue;
		}
		//
		std::vector<char> szTmp;
		szTmp.resize(1024);
		if(FD_ISSET(Detach(), &rset))
		{
			while(true)
			{
				nRet = Recv(&szTmp[0], 1024);
				if(m_pbufRecv->Append(&szTmp[0], nRet))
				{
					LogError("%s(%d)", __FILE__, __LINE__);
					return false;
				}
				if(errno == EAGAIN)
				{
					break;
				}
			}
			
			if(m_pbufRecv->GetBufLen() < sizeof(HEADER))
				continue;
			
			PHEADER pHeader = (PHEADER)m_pbufRecv->GetBufPtr();
			if(pHeader == nullptr)
			{
				return false;
			}
			if(pHeader->dwLength + sizeof(HEADER) >= m_pbufRecv->GetBufLen())
				break;
		}
	}

	//TODO Selcet mod 接受返回数据， 定时设置
	return true;
}


PHEADER CConnection::PeerMsg()
{
	if(m_pbufRecv == nullptr)
		return nullptr;
	if(m_pbufRecv->GetBufLen() < sizeof(HEADER))
		return nullptr;
	m_pHeader = (PHEADER) m_pbufRecv->GetBufPtr();
	if(nullptr == m_pHeader)
		return nullptr;
	return m_pHeader;
}
