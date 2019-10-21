#pragma once

#include "Notify.h"

#include <vector>
#include <string>
#include <string.h>

#include "Buffer.h"


class CMail : public CNotify
{
public:
	CMail();
	virtual ~CMail();
public:
	virtual bool OnInitialUpdate(const char* pszConfigFileName);

	virtual bool Notify(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle, const char* pszWarnInfo);	

	virtual bool CheckConfigure();
protected:	
	bool SendEmail(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle, const char* pszMailTxt);

	bool LogOn();

	bool LogOut();

protected:
	bool ConnectMailSvr();

	bool SendEmailHead(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle);

	bool SendEmailBody(std::vector<CONTACTORPtr>& listContactor, const char* pszMailTxt);	//发送文本信息
	
	bool SendAttachment();	//发送附件

	bool SendEmailEnd();	//发送邮件结束信息 
	
	int  SendMsg(const void* pszBuf, unsigned int nLen, bool bWaitRecv = false, char* pszRecvBuf = nullptr, unsigned int nLength = 0);

protected:
	void FormatHeadMail(CBuffer* pBuffer,const char*  pszTitle, std::vector<CONTACTORPtr>& listContactor);
	
	void FormatBodyMail(CBuffer* pBuffer, const char* pszWarnInfo);

protected:
	char		m_szUserName[64];			//账户名
	char		m_szPassword[64];			//账户密码
	char		m_szDefaultSendMail[64];	//发件人邮箱地址
	char		m_szSenderSimpleName[64];	//发件人简称	
	char		m_szEmailSvrName[64];		//邮件服务器名称
	char		m_szEmailHostIP[32];		//邮件服务器地址
	char		m_szEhlo[64];				//申请身份认证
	int			m_nPort;					//邮件服务器端口号
	bool		m_bLoadCfg;					//加载配置完成
};
