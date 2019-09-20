
#include "Socket.h"


class CNetClient: public CSocket
{
public:
	CNetClient();
	virtual ~CNetClient();
public:
//	virtual  bool   OnMsg(PHEADER  pMsg) = 0;
	virtual  bool	OnConnect()	= 0;
	virtual	 bool	OnBreak() = 0;
//	virtual  bool	OnAckMsg(PHEADER pMsg) = 0;
public:

	bool	SendMsg(PHEADER pMsg);
	
	void	Terminate(bool bWhenSendOver = false);

	void	SetMsgOrigine(unsigned short usMsgOrigine);
	
	void	SetRecvIdleTime(int nSecond);
	
public:
	void	BindUserObject();//TODO
	
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

};
