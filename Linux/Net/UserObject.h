#include "NetClient.h"


class CUserObject
{
public:
	CUserObject();
	virtual ~CUserObject();
public:
	virtual bool	OnInitialUpdate();
	
public:
	virtual bool	OnConnect();				//网络建立连接
	virtual bool	OnMsg(PHEADER pMsg);		//网络消息到达接口
	virtual bool	OnBreak();					//网络连接断开事件接口
	virtual bool	OnTickCount();				//网络连接分钟定时调用
public:
	bool	SendMsg(PHEADER pMsg);

public:
	virtual void	BindNetClient(CNetClient* pNetClient);
	CNetClient*		GetNetClient();
	bool			IsBindNetClient();
	
public:
	void	SetNetUserIP(unsigned int  uUserIP);
	void	GetWaitMsgCount(int &cnSendWait, int & &cnRecvWait);
protected:
	
	CNetClient*		m_pNetClient;

	unsigned short	m_uOrigine;
	unsigned int	m_uUserIP;
	char			m_szUserIP[32];

	time_t			m_timeConnect;
	time_t			m_timeBreak;
	
	int				m_nEnterPort;
};
