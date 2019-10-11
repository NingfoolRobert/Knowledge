#pragma once 

//#include "Socket.h"
#include "Notify.h"

class CMail : public CNotify
{
public:
	CMail();
	virtual ~CMail();
public:
	virtual bool OnIntialUpdate(const char* pszConfigFileName);

	virtual bool OnNotify();
public:
	
	bool GetRecvMailAddr(std::vector<std::string>& listRecvMailContactor);

public:
	bool LogOn();

	bool LogOut();

	bool SendMail(std::vector<std::string>& listContactor,const char* pszMailTxt);

protected:
	bool SendEmailHead();	//放邮件头部信息
	
	bool SendEmailBody();	//发送文本信息
	
	bool SendAttachment();	//发送附件

	bool SendEmailEnd();	//发送邮件结束信息 

protected:
	char		m_szUserName[64];			//账户名
	char		m_szSecret[64];				//账户密码
	char		m_szDefaultSendMail[64];	//默认发送邮件账户名
	char		m_szEmailSvrName[64];		//邮件服务器名称
private:
	
};
