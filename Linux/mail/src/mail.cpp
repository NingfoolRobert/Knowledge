#include <iostream>
#include <fstream>

#include "tinyxml2.h"
#include "mail.h"

#include "Log.h"
#include "CommonHelper.h"

using namespace std;


CMail::CMail():m_nPort(25)
{
	m_nType = NOTIFY_TYPE_EMAIL;
}

CMail::~CMail()
{

}

bool CMail::OnIntialUpdate(const char* pszConfigFileName)
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
		strcpy(m_szEmailSvrName, pNotify->Attribute("EmailSvrName"));
		strcpy(m_szUserName, pNotify->Attribute("Account"));
		strcpy(m_szPassword, pNotify->Attribute("Passwd"));
		strcpy(m_szDefaultSendMail, pNotify->Attribute("SenderMail"));
		if(0 == strlen(m_szDefaultSendMail))
		{
			LogWarn("Not Configure the Sender email.");
			return false;
		}
		strcpy(m_szSenderSimpleName, pNotify->Attribute("SimpleName"));
		
		m_nPort = atoi(pNotify->Attribute("Port"));
	
	
//	//TODO  获取配置信息
//	try
//	{
//		ifstream in(pszConfigFileName,ios::binary);
//		if(!in.is_open())
//		{
//			return false;
//		}
//	
//		Json::Reader reader;
//		Json::Value	 root;
//		
//		reader.parse(in, root);
//		
//		if(!root.isMember("mailSvr"))
//		{
//			return false;
//		}
//		string SenderSimpleName = root["mailSvr"]["SenderSimpleName"].asString();
//	
//		string SenderEmail = root["mailSvr"]["SenderEmail"].asString();
//		memcpy(m_szDefaultSendMail,SenderEmail.c_str(), SenderEmail.length());
//		
//		string strAccount = root["mailSvr"]["Account"].asString();
//		memcpy(m_szUserName, strAccount.c_str(), strAccount.length());
//		
//		string strSecret  = root["mailSvr"]["Secret"].asString();
//		memcpy(m_szSecret, strSecret.c_str(), strSecret.length());
//
//		string strEmailSvrName  = root["mailSvr"]["Name"].asString();
//		memcpy(m_szEmailSvrName, strEmailSvrName.c_str(), strEmailSvrName.length());
//		m_nPort = root["mailSvr"]["Port"].asInt();
		
	}
	catch(std::exception e)
	{
		LogWarn("Parse Configure file(%s) fail...", pszConfigFileName);
	}

	if(!ConnectMailSvr())
	{
		return false;
	}
	if(!LogOn())
	{
		return false;
	}

	return true;
}

bool CMail::SendNotify()
{
	return true;
}

bool CMail::Notify(std::vector<CONTACTORPtr>& listContactor, const char* pszWarnInfo)
{
	if(nullptr == pszWarnInfo || 0 == strlen(pszWarnInfo))
		return false;
	std::string strRecvier = GenMailReciver(listContactor);
	if(0 == strRecvier.length())
	{

		LogWarn("Recvier Count: %d", listContactor.size());
		return false;
	}

	return true;
}

bool CMail::SendEmail(std::string strRecvier, const char* pszWarnInfo)
{
	if( 0 == strRecvier.length() || nullptr == pszWarnInfo || 0 == strlen(pszWarnInfo) )
		return false;


	//Email head;
	int nRet = SendEmailHead();	
	//email body 
	nRet = SendEmailBody(strRecvier, pszWarnInfo);
	//email end 

	return true;
}

bool CMail::ConnectMailSvr()
{
	if(0 == strlen(m_szEmailSvrName))
	{
		return false;
	}	
	struct hostent *host = gethostbyname(m_szEmailSvrName);
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
	//int nRet = RecvMsg(szTmp, 256);
	while(RecvMsg(szBuffer, 256) == 0)
	{
		Connect(szTmp, m_nPort);
		LogWarn("Connect mail svr. IP:Port = %s:%d", m_szEmailHostIP, m_nPort);
		usleep(1000 * 1000);
	}
	LogInfo("mail Svr Return: %s", szBuffer);

	return true;
}


bool CMail::LogOn()
{
	//申请身份认证
	char szTmp[1024] = {0};
	sprintf(szTmp,"%s","EHLO CICC\GWGroup\r\n");
	if(!SendNotifyInfo(szTmp, strlen(szTmp)))
	{
		return false;
	}
	//Recv Data;
	char szRecv[1024]= {0};
	//Recv Data;
	int nRet = RecvMsg(szRecv, sizeof(szRecv));
//	if(strncmp(szRecv,"250", 3) != 0)
//	{
//		return false;
//	}
	//请求登录
	memset(szTmp,0,sizeof(szTmp));
	sprintf(szTmp, "AUTH LOGIN\r\n");
	if(!SendNotifyInfo(szTmp,strlen(szTmp)))
	{
		return false;
	}
	//RecvData;
	memset(szRecv, 0, sizeof(szRecv));
	//RecvData
	nRet = RecvMsg(szRecv, sizeof(szRecv));
//	if(strncmp(szRecv, "334", 3) != 0)
//	{
//		return false;
//	}
	
	LogInfo("Start LogOn... EmailSvr: %s ", m_szSenderSimpleName);

	//

	char szLogInUserName[128]={0};
	char szLogPassword[128] ={0};

	
	CommonHelper::Char2Base64(szLogInUserName, m_szUserName, strlen(m_szUserName));
	CommonHelper::Char2Base64(szLogPassword, m_szPassword, strlen(m_szPassword));

	strcat(szLogInUserName, "\r\n");
	if(SendMsg(szLogInUserName, strlen(szLogInUserName)) < 0)
	{
		LogError("Send UserName fail: Net Error. ");
		return false;
	}
//	if(!SendNotifyInfo(szLogInUserName, strlen(szLogInUserName)))
//	{
//		LogWarn("LogOn fail.");
//		return false;
//	}


	strcat(szLogPassword, "\r\n");
//	if(!SendNotifyInfo(szLogPassword,  strlen(szLogPassword)))
//	{
//		LogWarn("LogOn Send password fail.");
//		return false;
//	}

	if(SendMsg(szLogPassword,strlen(szLogPassword), true, szRecv, sizeof(szRecv)) < 0)
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

bool CMail::SendMail(std::vector<std::string>& listContactor,const char* pszMailTxt)
{
	if(pszMailTxt == nullptr)
	{
		return false;
	}

	return true;
}



bool CMail::SendEmailHead(std::string strRecvier)
{
	char szTmp[1024] = { 0 };

//	std::string str = "MAIL FROM:<" + m_szDefaultSendMail + ">\r\n";
	sprintf(szTmp, "%s%s%s", "MAIL FROM:<", m_szDefaultSendMail, ">\r\n");
//	if(!SendNotifyInfo(str.c_str(), str.length()))
//	{
//		return false;
//	}
	//Recv Data;
	char szRecvMsg[1024] = { 0 };
	if(SendMsg(szTmp, strlen(szTmp), true, szRecvMsg, sizeof(szRecvMsg)) < 0)
	{
		LogWarn("%s(%d) Send Message fail.", __FILE__, __LINE__);
		return false;
	}
	memset(szTmp, 0, sizeof(szTmp));
	
	sprintf(szTmp,"RCPT TO:<%s>\r\n", m_szDefaultSendMail);
	
	if(!SendNotifyInfo(szTmp, strlen(szTmp)))
	{
		return false;
	}
	//Recv Data;
	
	memset(szTmp, 0, sizeof(szTmp));
	
	memcpy(szTmp, "DATA\r\n", strlen("DATA\r\n"));

	if(!SendNotifyInfo(szTmp,strlen(szTmp)))
	{
		return false;
	}
	//Recv Data;

	memset(szTmp, 0, sizeof(szTmp));
	

	return true;
}

bool CMail::SendEmailBody(const char* pszMailTxt, int nLen)	//发送文本信息
{

	char szTmp[2048] ={0};
	
	//Email Body 
	strcpy(szTmp, "DATA\r\n");
	char szRecv[1024] = { 0 }; 
	if(SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szTmp)) < 0)
	{
		LogWarn("Send Data head fail.");
		return false;
	}
	
	memset(szRecv, 0, sizeof(szRecv));
	std::vector<char> MailBodyInfo;
	MailBodyInfo.resize(nLen + 32);
	sprintf(&MailBodyInfo[0],  "%s%s", pszMailTxt, "\r\n.\r\n");
	if(SendMsg(&MailBodyInfo[0], strlen(&MailBodyInfo[0]), true, szRecv,sizeof(szRecv)) < 0)
	{
		LogError("Send Mail body information error.");
		return false;
	}
	//
	memset(szRecv, 0, sizeof(szRecv));
	strcpy(szTmp, "QUIT\r\n");
	if(SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv)) < 0)
	{
		LogWarn("Send Mail End fail.");
		return false;
	}
	
	return true;
}

bool CMail::SendAttachment()	//发送附件
{
	//TODO 
	
	return true;
}

bool CMail::SendEmailEnd()	//发送邮件结束信息 
{
	char szTmp[1024]={0};
	sprintf(szTmp, "--INVT--\r\n.\r\n");
	
	SendNotifyInfo(szTmp, strlen(szTmp));
	
	sprintf(szTmp, "QUIT\r\n");
	SendNotifyInfo(szTmp, strlen(szTmp));

	return true;
}

std::string  CMail::GenMailReciver(std::vector<CONTACTORPtr>& listContactor)
{
	std::string str;

	auto it = listContactor.end();
	auto pUser = *it;
	for(; it != listContactor.begin(); --it)
	{
		std::string strName = pUser->szName;
		std::string strEmail = pUser->szEmail;
		str += strName + "<" + strEmail +">" + ";";
	}

	return str;
}



int CMail::SendMsg(const void* pszBuf, unsigned int nLen, bool bWaitRecv/* = false*/, char* pszRecvBuf/* = nullptr*/, unsigned int nLength/* = 0*/)
{
	if(pszBuf == nullptr || 0 == nLen)
	{
		return -1;
	}
	
	unsigned int nRet = SendNotifyInfo((char*)pszBuf, nLen);
	if(nRet != nLen)
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
