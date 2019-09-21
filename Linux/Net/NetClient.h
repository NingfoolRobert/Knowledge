#pragma once
#include "Socket.h"
#include "Protocol.h"
#include "Buffer.h"
#include "ConcurrentQueueNew.h"

class CUserObject;

class CNetClient: public CSocket
{
public:
	friend class CUserObject;
	CNetClient();
	virtual ~CNetClient();
public:
	virtual  bool   OnMsg(PHEADER  pMsg);
	virtual  bool	OnConnect();
	virtual	 bool	OnBreak();
	virtual  bool	OnAckMsg(PHEADER pMsg);
	virtual  bool	OnTickCount();
public:

	bool	SendMsg(PHEADER pMsg);
	
	void	Terminate(bool bWhenSendOver = false);

	void	SetMsgOrigine(unsigned short usMsgOrigine);
	
	void	SetRecvIdleTime(int nSecond);
	
public:
	void	BindUserObject(CUserObject* pUser);//TODO
	
	bool	IsBindUserObject();

private:
	bool	OnSend();
	bool	OnReceive();
	bool	OnActiveMsg();
public:
	struct sockaddr_in		m_addr;			//客户端连接地址
	unsigned int			m_uRemoteIP;	
	char					m_szIP[32];
	int						m_nEnterPort;	//连接端口
	


private:
	//Buffer
	//
	CUserObject*			m_pUserObject;
	CICCTools::ConcurrentQueueNew<CBuffer*>  m_listSend;
	CICCTools::ConcurrentQueueNew<CBuffer*>	 m_listRecv;
};
