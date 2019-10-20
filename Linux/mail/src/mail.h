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

	virtual bool Notify(std::vector<CONTACTORPtr>& listContactor, const char* pszWarnInfo);	
public:
	
	bool SendEmail(std::string strRecvier, const char* pszWarnInfo);
//	bool GetRecvMailAddr(std::vector<std::string>& listRecvMailContactor);

public:

	bool LogOn();

	bool LogOut();

	bool SendMail(std::vector<std::string>& listContactor,const char* pszMailTxt);

protected:
	bool ConnectMailSvr();

	bool SendEmailHead(std::vector<std::string> &listContactor);	//放邮件头部信息

	bool SendEmailHead(std::string strRecvier);

	bool SendEmailBody(const char* pszMailTxt, int nLen);	//发送文本信息
	
	bool SendAttachment();	//发送附件

	bool SendEmailEnd();	//发送邮件结束信息 
	
	bool GeneratNotifyInfo(std::string& pstr/*,LogWarning* pWarnInfo*/);		//	生成报警文本信息

	std::string  GenMailReciver(std::vector<CONTACTORPtr>& listContactor);

	int  SendMsg(const void* pszBuf, unsigned int nLen, bool bWaitRecv = false, char* pszRecvBuf = nullptr, unsigned int nLength = 0);
protected:
	char		m_szUserName[64];			//账户名
	char		m_szPassword[64];			//账户密码
	char		m_szDefaultSendMail[64];	//发件人邮箱地址
	char		m_szSenderSimpleName[64];	//发件人简称	
	char		m_szEmailSvrName[64];		//邮件服务器名称
	char		m_szEmailHostIP[32];		//邮件服务器地址
	int			m_nPort;					//邮件服务器端口号
};
