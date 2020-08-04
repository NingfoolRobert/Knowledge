#include "TcpClient.h"
#include "IOMgr.h"
#include <string.h> 
#include "GFunc.h"
#include "LogFile.h"
#include <unistd.h>


CTcpClient::CTcpClient()
{
	
}
	
CTcpClient::~CTcpClient()
{

}

bool CTcpClient::OnMsg(PHEADER& pMsg)
{
	return true;
}

void CTcpClient::OnBreak()
{

}

void CTcpClient::OnTickCnt()
{
	
}
	
bool CTcpClient::Init(const char*  pszHostIP, int nPort, bool bAutoIOMgr/* = false*/)
{
	if(nullptr == pszHostIP ||  15 < strlen(pszHostIP))
		return false; 
	
	unsigned int dwHostIP =  IPStr2Host(pszHostIP);//Str2HostIP(pszHostIP);
	
	return Init(dwHostIP, nPort, bAutoIOMgr);
}
	
bool CTcpClient::Init(unsigned int dwHostIP, int nPort, bool bAutoIOMgr/* = false*/)
{
	m_dwHostIP = dwHostIP;
	m_nPort = nPort;
	m_bAutoIOMgr = bAutoIOMgr;

	if(nullptr == m_pOwner)
	{
		CStreamIO* pStreamIO  = new CStreamIO;
		if(nullptr == pStreamIO)
		{
			char szIP[16] = { 0 };
			LogError("memmory error. HostIP:Port=%s:%d", HostIP2Str(dwHostIP, szIP), nPort);
			return false;
		}
		pStreamIO->m_dwIP	= m_dwHostIP;
		pStreamIO->m_nPort	= m_nPort;
		m_pOwner = pStreamIO;
	}	
	
	int fd = m_pOwner->Create();
	if(fd < 0)
	{
		char szIP[16] = { 0 };
		LogError("Create Socket fail. errno:%d,  HostIP:Port=%s:%d", errno, HostIP2Str(dwHostIP, szIP), nPort);
		return false;
	}
	
	if(!m_pOwner->Connect(m_dwHostIP, m_nPort))
	{
		char szIP[16] = { 0 };
		LogError("Connect fail. errno:%d,  HostIP:Port=%s:%d", errno, HostIP2Str(dwHostIP, szIP), nPort);
		return false;
	}

	if(!CActiveIO::OnInit(m_pOwner))
		return false;

	//自动执行
	if(m_bAutoIOMgr)
	{
		if(g_pIOMgr)
			return g_pIOMgr->AddNetIO(this);		//添加到IO管理模块
		return false;
	}

	return true;
}

//重连
bool CTcpClient::Reconnect()
{
	if(m_pOwner)
		m_pOwner->Close();
	
	m_tBreak = 0;
	m_tLastestMsg = 0;
	return Init(m_dwHostIP, m_nPort, m_bAutoIOMgr);	
}
	
bool CTcpClient::SendSyncMsg(PHEADER pMsg, Buffer* pBuf)
{
	if(nullptr == pMsg || nullptr == pBuf)
	{
		return false;	
	}
	
	if(!SendMsg(pMsg))
		return false;
	
	
	return CActiveIO::PeerMsg(pBuf);
}
