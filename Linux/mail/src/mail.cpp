#include "tinyxml2.h"
#include "mail.h"
#include "Log.h"
#include "CommonHelper.h"
#include <string.h>
#include <memory.h>


CMail::CMail():m_nPort(25),m_bLoadCfg(false)
{
	memset(m_szEhlo, 0, sizeof(m_szEhlo));
	strcpy(m_szEhlo, "EHLO CICCGWGroup");
	strcpy(m_szUserName, "it_support_app_sende");
	strcpy(m_szPassword, "setupandindex");
	memset(m_szEmailHostIP, 0, sizeof(m_szEmailHostIP));
	memset(m_szEmailSvrName, 0, sizeof(m_szEmailSvrName));
	strcpy(m_szSenderSimpleName, m_szUserName);
	//memset(m_szDefaultSendMail, 0, sizeof(m_szDefaultSendMail));
	sprintf(m_szDefaultSendMail,"%s@cicc.com.cn", m_szUserName);

	m_nType = NOTIFY_TYPE_EMAIL;
}

CMail::~CMail()
{

}

bool CMail::OnInitialUpdate(const char* pszConfigFileName)
{
	if(!CNotify::OnInitialUpdate(pszConfigFileName))
	{
		return false;
	}

	using namespace tinyxml2;
	try 
	{
		XMLDocument doc;
		if(doc.LoadFile(pszConfigFileName))
		{
			LogError("Load Configure File(%s) fail...", pszConfigFileName);
			return false;
		}
	
		XMLElement* pService = doc.FirstChildElement("Service");
		if(nullptr == pService)
		{
			LogError("Configure file(%s) not find AlaramService Node.", pszConfigFileName);
			return false;
		}
	
		XMLElement* pNotify = pService->FirstChildElement("Notify");
		if(nullptr == pNotify)
		{
			return false;
		}
		XMLElement* pEmail = pNotify->FirstChildElement("mail");
		if(nullptr == pEmail)
		{
			return false;
		}
		char szTmp[128] = { 0 };
		
		memset(szTmp, 0, sizeof(szTmp));
	
		auto pAttr  = pEmail->Attribute("Account");
		if(nullptr == pAttr ||  0 == strlen(pAttr))
		{
			LogWarn("Default Account Name: %s", m_szUserName);
			if(0 == strlen(m_szUserName))
				return false;
		}
		else 
		{
			strcpy(m_szUserName, pAttr);
		}
		
		memset(szTmp, 0, sizeof(szTmp));
		auto pPasswd = pEmail->Attribute("Passwd");
		if(nullptr == pPasswd || 0 == strlen(pPasswd))
		{
			LogWarn("Default Passwd: %s", m_szPassword);
			if(0 == strlen(m_szPassword))
				return false;
		}
		else 
		{
			strcpy(m_szPassword, pPasswd);
		}
		auto pHost = pEmail->Attribute("EmailHost");
		if( nullptr == pHost || 0 == strlen(pHost) )
		{
			LogWarn("Default Host : %s", m_szEmailHostIP);
			if(0 == strlen(m_szEmailHostIP))
				return false;
		}
		else 
		{
			strcpy(m_szEmailHostIP, pHost);
		}
		
		auto pSenderEmail = pEmail->Attribute("SenderEmail");
		if( nullptr == pSenderEmail || 0 == strlen(pSenderEmail) )
		{
			LogWarn("Default SenderEmail : %s", m_szDefaultSendMail);
			if(0 == strlen(m_szDefaultSendMail))
				return false;
		}
		else 
		{
			strcpy(m_szDefaultSendMail, pSenderEmail);
		}
		

		memset(szTmp, 0, sizeof(m_szEhlo));
		auto  pEhlo = pEmail->Attribute("Ehlo");
		if(pEhlo == nullptr || 0 == strlen(pEhlo))
		{
			LogWarn("Default Ehlo : %s", m_szEhlo);
			if(0 == strlen(m_szEhlo))
				return false;
		}
		else 
		{
			strcpy(m_szEhlo, pEhlo);
		}
		auto pSimpleName = pEmail->Attribute("SimpleName");
		if(nullptr == pSimpleName || 0 == strlen(pSimpleName))
		{
			LogWarn("Default SenderSimpleName : %s", m_szSenderSimpleName);
		}
		else 
		{
			strcpy(m_szSenderSimpleName, pSimpleName);
		}
		//
		m_nPort = pEmail->IntAttribute("Port",25);

		m_bLoadCfg = true;
	}
	catch(std::exception e)
	{
		LogWarn("Parse Configure file(%s) fail...", pszConfigFileName);
	}

	return true;
}

//bool CMail::Notify(std::vector<CONTACTORPtr>& listContactor,const char* pszTitle, const char* pszWarnInfo)
bool CMail::Notify(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle, CBuffer* pBuffer)	
{
	if(nullptr == pBuffer)
		return false;
	if(!m_bLoadCfg)
	{
		LogError("Not Finished the Email Svr Configure.");
		return false;
	}
	if(!ConnectMailSvr())
	{
		LogWarn("Connect Email Svr fail. WarnningInfo:%s", pBuffer->GetBufPtr());
		return false;
	}
	if(!LogOn())
	{
		return false;
	}
	SendEmail(listContactor, pszTitle, pBuffer);
	//
	Close();
	return true;
}

bool CMail::CheckConfigure()
{
	
	return true;
}
//
bool CMail::ConnectMailSvr()
{
	struct hostent *host = gethostbyname(m_szEmailHostIP);
	if(nullptr == host)
	{
		return false;
	}
	char szTmp[32] = { 0 };;
	if(host->h_addrtype == AF_INET)
	{
		inet_ntop(host->h_addrtype, host->h_addr_list[0],szTmp, sizeof(szTmp));
	}
	if(!Connect(szTmp, m_nPort))
	{
		LogError("%(%d) Connect mail Svr fail. IP:Port = %s:%d", __FILE__, __LINE__, m_szEmailHostIP, m_nPort);
		return false;
	}
	char szBuffer[256] = { 0 };
	RecvMsg(szBuffer, 256);
	LogInfo("mail Svr Return: %s", szBuffer);

	return true;
}


bool CMail::LogOn()
{
	//申请身份认证
	char szTmp[1024] = {0};
	//sprintf(szTmp,"%s","EHLO CICC\GWGroup\r\n");
	strcat(m_szEhlo, "\r\n");
	if(SendMsg(m_szEhlo, strlen(m_szEhlo), true, szTmp, sizeof(szTmp)) < 0)
	{
		LogError("Send Ehlo fail.");
		return false;
	}

	if(strstr(szTmp, "503") != nullptr || strstr(szTmp, "500") != nullptr)
	{
		LogError("Email Error: %s", szTmp);
		return false;
	}

	//请求登录
	memset(szTmp,0,sizeof(szTmp));
	sprintf(szTmp, "AUTH LOGIN\r\n");
	if(SendMsg(szTmp, strlen(szTmp), true, szTmp, sizeof(szTmp)) < 0)
	{
		LogError("Request Auth LogIn fail.");
		return false;
	}
	
	LogInfo("Start LogIn... EmailSvr: %s ", m_szSenderSimpleName);

	//

//	strcpy(m_szUserName, "it_support_app_sende");
//	strcpy(m_szPassword,"setupandindex");
//	char szLogInUserName[128]={"it_support_app_sende"};
//	char szLogPassword[128] ={"setupandindex"};

	char szUserName[128]={0};
	char szPassword[128] ={0};
	
	CommonHelper::Char2Base64(szUserName, m_szUserName, strlen(m_szUserName));
	CommonHelper::Char2Base64(szPassword, m_szPassword, strlen(m_szPassword));

	strcat(szUserName, "\r\n");
	if(SendMsg(szUserName, strlen(szUserName)) < 0)
	{
		LogError("Send UserName fail: Net Error. ");
		return false;
	}
	strcat(szPassword, "\r\n");
	char szRecv[256] = { 0 };
	if(SendMsg(szPassword,strlen(szPassword), true, szRecv, sizeof(szRecv)) < 0)
	{
		LogWarn("Send User Passwd fail.");
		return false;
	}
	//
	if(nullptr != strstr(szRecv,"550"))
	{
		LogError("Log on fail: UserName Error.");
		return false;
	}
	else if(nullptr != strstr(szRecv, "535"))
	{
		LogError("Log on fail: UserPasswd Error.");
		return false;
	}

	return true;
}

bool CMail::LogOut()
{
	return true;
}

bool CMail::SendEmail(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle, CBuffer* pBuffer)
//bool CMail::SendEmail(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle, const char* pszMailTxt)
{
	if(nullptr == pBuffer)
	{
		return false;
	}

	bool bRet = SendEmailHead(listContactor, pszTitle);
	if(!bRet)
	{
		LogError("%s(%d) Send Email fail. WarnningInfo: %s", __FILE__, __LINE__, pBuffer->GetBufPtr());
		return false;
	}

	bRet = SendEmailBody(listContactor,pBuffer);
	if(!bRet)
	{
		LogError("%s(%d) Send Email Body fail. WarnningInfo: %s", __FILE__, __LINE__, pBuffer->GetBufPtr());
		return false;
	}
	bRet = SendEmailEnd();
	if(!bRet)
	{
		LogError("%s(%d) Send Email End fail. WarnningInfo: %s", __FILE__, __LINE__, pBuffer->GetBufPtr());
		return false;
	}
	return true;
}


bool CMail::SendEmailHead(std::vector<CONTACTORPtr>& listContactor, const char* pszTitle)
{
	char szTmp[1024] = { 0 };
	char szRecv[1024] = { 0 };
	sprintf(szTmp, "%s%s%s", "MAIL FROM:<", m_szDefaultSendMail, ">\r\n");
	
	char szRecvMsg[1024] = { 0 };
	if(SendMsg(szTmp, strlen(szTmp), true, szRecvMsg, sizeof(szRecvMsg)) < 0)
	{
		LogWarn("%s(%d) Send Message fail.", __FILE__, __LINE__);
		return false;
	}

	//Add Recvier 
	for(auto i = 0; i < listContactor.size(); ++i)
	{
		memset(szTmp, 0, sizeof(szTmp));
		sprintf(szTmp,"RCPT TO:<%s>\r\n", listContactor[i]->szEmail);
		memset(szRecv, 0, sizeof(szRecv));
		if(SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv)) < 0)
		{
			LogWarn("Add RCPT User fail: UserName:%s No:%d", listContactor[i]->szName, listContactor[i]->nNO);
			continue;
		}
	}
	
	//
	memset(szRecv, 0, sizeof(szRecv));
	CBuffer stBuf;
	FormatHeadMail(&stBuf, pszTitle, listContactor);
	if(SendMsg(stBuf.GetBufPtr(), stBuf.GetBufLen(), true, szRecv, sizeof(szRecv)) < 0 )
	{
		LogError("Send Email Header fail.");
		return false;
	}
	
	return true;
}

//bool CMail::SendEmailBody(std::vector<CONTACTORPtr>&listContactor, const char* pszMailTxt)	//发送文本信息
bool CMail::SendEmailBody(std::vector<CONTACTORPtr>& listContactor, CBuffer* pBuffer)	//发送文本信息
{
	CBuffer stBuf;
	FormatBodyMail(&stBuf, pBuffer->GetBufPtr());
	if(SendMsg(stBuf.GetBufPtr(), stBuf.GetBufLen() ) < 0)
	{
		LogError("Send Mail body information error.");
		return false;
	}
	//
	return true;
}

bool CMail::SendAttachment()	//发送附件
{
	//TODO 
	
	return true;
}

bool CMail::SendEmailEnd()	//发送邮件结束信息 
{
	char szTmp[256]={0};
	strcpy(szTmp, "\r\n\r\n该邮件由ETL系统自动发出，如有疑问，请联系ETL相关人员\r\n.\r\n");
	char szRecv[256] = { 0 };
	if(SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv)) < 0)
	{
		LogError("Send Email End Error.");
		return false;
	}
	strcpy(szTmp, "QUIT\r\n");
	if(SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv)) < 0)
	{
		LogWarn("Send Mail End fail.");
		return false;
	}
	return true;
}

int CMail::SendMsg(const void* pszBuf, unsigned int nLen, bool bWaitRecv/* = false*/, char* pszRecvBuf/* = nullptr*/, unsigned int nLength/* = 0*/)
{
	if(pszBuf == nullptr || 0 == nLen)
	{
		return -1;
	}
	
	unsigned int nRet = SendNotifyInfo((char*)pszBuf, nLen);
	if(!nRet)
	{
		return -1;
	}
	if(!bWaitRecv)
	{
		return 0;
	}

	// Sync RecvierMsg;
	if(nullptr == pszRecvBuf || 0 == nLen)
	{
		LogWarn("Recv Msg error.");
		return 0;
	}

	nRet = RecvMsg(pszRecvBuf ,nLength);
	
	return nRet;
}

void CMail::FormatHeadMail(CBuffer* pBuffer, const char* pszTitle, std::vector<CONTACTORPtr>& listContactor)
{

	char szTmp[256] = { 0 };
	strcat(szTmp, "DATA\r\n");
	if(!pBuffer->Append(szTmp, strlen(szTmp)))
	{
		LogError("%s(%d) Append String Error.", __FILE__, __LINE__);
		return ;
	}

	memset(szTmp, 0, sizeof(szTmp));
	sprintf(szTmp, "From: %s<%s>\r\n", m_szSenderSimpleName, m_szDefaultSendMail);
	if(!pBuffer->Append(szTmp, strlen(szTmp)))
	{
		LogError("%s(%d) Append String Error.", __FILE__, __LINE__);
		return ;
	}

	if(!pBuffer->Append("To: ", 4))
	{
		LogError("%s(%d) Append String Error.", __FILE__, __LINE__);
		return ;
	}
	for(auto i = 0; i < listContactor.size(); ++i)
	{
		memset(szTmp, 0, sizeof(szTmp));

		sprintf(szTmp, "%s<%s>;", listContactor[i]->szName, listContactor[i]->szEmail);
		if(!pBuffer->Append(szTmp, strlen(szTmp)))
		{
			LogError("%s(%d) Append String Error.", __FILE__, __LINE__);
			return ;
		}
	}
	if(!pBuffer->Append("\r\n ", 2))
	{
		LogError("%s(%d) Append String Error.", __FILE__, __LINE__);
		return ;
	}
	//
	memset(szTmp, 0, sizeof(szTmp));
	
	sprintf(szTmp, "Subject: %s\r\n", pszTitle);
	
	if(!pBuffer->Append(szTmp,strlen(szTmp)))
	{
		LogError("%s(%d) Append String Error.", __FILE__, __LINE__);
		return ;
	}

	memset(szTmp, 0, sizeof(szTmp));
	strcpy(szTmp, "MIME-Version 1.0\r\n\r\n");
	if(!pBuffer->Append(szTmp, strlen(szTmp)))
	{
		LogError("%s(%d) Append String Error.", __FILE__, __LINE__);
		return ;
	}

//	memset(szTmp, 0, sizeof(szTmp));
//	sprintf(szTmp, "%s%s", "Content-Type: multipart/mixed,boundary=qwertyuiop","\r\n");
//	if(!pBuffer->Append(szTmp, strlen(szTmp)))
//	{
//		LogError("%s(%d) Append String Error.", __FILE__, __LINE__);
//		return ;
//	}
	
}

void CMail::FormatBodyMail(CBuffer* pBuffer, const char* pszWarnInfo)
{
	pBuffer->Clear();
	char szTmp[256] = { 0 };
	//strcat(szTmp, "--qwertyuiop\r\n");
	//strcat(szTmp, "Content-Type: text/plain");
	//strcat(szTmp, "charset=\"gb2312\"\r\n\r\n");
	strcpy(szTmp, "\r\n ETL报警通知 \r\n\r\n");
	if(!pBuffer->Append(szTmp, strlen(szTmp)))
	{
		LogError("%s(%d) Append Data error." , __FILE__, __LINE__);
		return ;
	}
	
	if(!pBuffer->Append(pszWarnInfo, strlen(pszWarnInfo)))
	{
		LogError("%s(%d) Append Data error." , __FILE__, __LINE__);
		return ;
	}

	if(!pBuffer->Append("\r\n", strlen("\r\n")))
	{
		LogError("%s(%d) Append Data error." , __FILE__, __LINE__);
		return ;
	}
}
