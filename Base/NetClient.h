/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		NetClient.h
 * @brief:		客户端网络IO收发消息
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-10-24
 */

#pragma once

#include "NetIO.h"
#include "Buffer.h"
#include "AutoLock.h"
#include "Protocol.h"

#include <atomic> 
#include <queue>

#define EPOLLCLOSE				0x80000000		//关闭套接字 





class CUserObject;
class CNetIOMgr;
class CNetService;


class CNetClient: public CNetIO 
{
	friend class CNetService;
public:
	CNetClient();

	virtual ~CNetClient();
public:
	virtual bool OnInitialUpdate(CNetService* pNetService, int fd);		//初始化Socket, UserObject, 
	
	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnSecondIdle();

	virtual bool OnTerminate();

public:
	
	virtual void OnRecv();		

	virtual void OnSend();

	virtual void OnClose();
public:
	
	void SendThread();				//发送消息线程

	void RecvThread();				//接受线程 

public:
	virtual bool OnMsg(PHEADER pMsg);		//消息到达网络端口

	virtual bool OnConnect();				//链接消息网络接口
	
	virtual bool OnBreak();					//网络断开

public:
	
	bool SendMsg(PHEADER pMsg);

	bool SendZipMsg(PHEADER pMsg);

	bool SendMsg(CBuffer* pBuf);

	bool Terminate();

	void BindUserObj(CUserObject* pUser);

	bool IsBindUserObject();

	void SetOrigin(int wOrigin);
public:
	int  RecvMsg();
	
	bool SendMsg();

protected:
	void ProcessMsg();
	//客户端连接的地址信息
public:
	struct sockaddr_in		m_addr;
	char					m_szIP[32];
	int						m_nPort;		//对端Port;
	unsigned int			m_dwIP;			//对端IP;

protected:


private:
	CUserObject*				m_pUserObj;
	CNetService*				m_pNetSercie;
private:

	int							m_wOrigin;
private:
	CObjectLock					m_clsRecv;	
	std::queue<CBuffer*>		m_listRecvMsg;
	CBuffer*					m_pbufRecv;
	std::atomic<unsigned int>	m_dwRecvSerial;

	CObjectLock					m_clsSend;
	std::queue<CBuffer*>		m_listSendMsg;
	CBuffer*					m_pbufSend;
	unsigned int				m_dwSendSerial;
	unsigned int				m_dwSendLeft;		//当前剩余发送数据字节
	unsigned int				m_dwSendOver;		//当前已发送数据字节
	unsigned int				m_dwSendBlockSize;	//总共需要发送数据字节数 
};
