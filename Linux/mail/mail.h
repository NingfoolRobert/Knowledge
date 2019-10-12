#pragma once 

#include "Notify.h"

#include <vector>
#include <string>
#include <string.h>


class CMail : public CNotify
{
public:
	CMail();
	virtual ~CMail();
public:
	virtual bool OnIntialUpdate(const char* pszConfigFileName);

	virtual bool SendNotify();
public:
	
//	bool GetRecvMailAddr(std::vector<std::string>& listRecvMailContactor);

public:

	bool LogOn();

	bool LogOut();

	bool SendMail(std::vector<std::string>& listContactor,const char* pszMailTxt);

protected:
	bool ConnectMailSvr();

	bool SendEmailHead();	//放邮件头部信息
	
	bool SendEmailBody(const char* pszMailTxt, int nLen);	//发送文本信息
	
	bool SendAttachment();	//发送附件

	bool SendEmailEnd();	//发送邮件结束信息 
	
	bool GeneratNotifyInfo(std::string& pstr/*,LogWarning* pWarnInfo*/);		//	生成报警文本信息

protected:
	char		m_szUserName[64];			//账户名
	char		m_szSecret[64];				//账户密码
	char		m_szDefaultSendMail[64];	//默认发送邮件账户名
	char		m_szEmailSvrName[64];		//邮件服务器名称
	int			m_nPort;					// 邮件服务器端口号
private:
	
};
