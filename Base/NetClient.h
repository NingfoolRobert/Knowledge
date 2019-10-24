/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		NetClient.h
 * @brief:		管理网络IO收发消息
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-24
 */

#pragma once

#include "Socket.h"
#include "Buffer.h"
#include "AutoLock.h"
#include "Protocol.h"


#include <queue>




class CUserObject;
class CNetIOMgr;


class CNetClient: public CSocket
{
public:
	CNetClient();

	virtual ~CNetClient();
public:
	virtual bool OnInitialUpdate(int fd, CNetIOMgr* pMgr);		//初始化Socket, UserObject, 
	
	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual bool OnTerminate();

public:
	
public:
	virtual bool OnMsg(PHEADER pMsg);		//消息到达网络端口

	virtual bool OnConnect();				//链接消息网络接口
	
	virtual bool OnBreak();					//网络断开

public:
	
	bool SendMsg(PHEADER pMsg);

	bool SendZipMsg(PHEADER pMsg);
	
	bool Terminate();

	void BindUserObj(CUserObject* pUser);

public:

	bool RecvMsg();
	
	bool SendMsg();

protected:
	CSocket					m_Socket;

	int						m_nPort;		//对端Port;
	unsigned int			m_dwIP;			//对端IP;

protected:
	

private:
	CUserObject*			m_pUserObj;

private:
	
private:
	CObjectLock				m_clsRecv;	
	std::queue<CBuffer*>	m_listRecvMsg;
	CBuffer*				m_bufRecv;
	std::atomic<unsigned int> m_dwRecvSerial;

	CObjectLock				m_clsSend;
	std::queue<CBuffer*>	m_listSendMsg;
	CBuffer*				m_bufSend;
	std::atomic<unsigned int>	m_dwSendSerial;
};
