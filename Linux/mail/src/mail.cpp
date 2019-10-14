#include <iostream>
#include <fstream>

#include "tinyxml2.h"
#include "mail.h"

using namespace std;


CMail::CMail():m_nPort(25)
{

}

CMail::~CMail()
{

}

bool CMail::OnIntialUpdate(const char* pszConfigFileName)
{
	if(!CNotify::OnIntialUpdate(pszConfigFileName))
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
	
		XMLElement* pService = doc.FirstChildElement("AlaramService");
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
		strcpy(m_szSecret, pNotify->Attribute("Secret"));
		strcpy(m_szDefaultSendMail, pNotify->Attribute("SenderMail"));
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
	return Connect(szTmp, m_nPort);
}


bool CMail::LogOn()
{
	char szTmp[1024] = {0};
	sprintf(szTmp, "HELO []\r\n");
	if(!SendNotifyInfo(szTmp, strlen(szTmp)))
	{
		return false;
	}
	//Recv Data;
	char szRecv[1024]= {0};
	//Recv Data;
	if(strncmp(szRecv,"250", 3) != 0)
	{
		return false;
	}
	memset(szTmp,0,sizeof(szTmp));
	sprintf(szTmp, "AUTH LOGIN\r\n");
	if(!SendNotifyInfo(szTmp,strlen(szTmp)))
	{
		return false;
	}
	//RecvData;
	memset(szRecv, 0, sizeof(szRecv));
	//RecvData;
	if(strncmp(szRecv, "334", 3) != 0)
	{
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



bool CMail::SendEmailHead(std::vector<std::string> &listContactor)	//放邮件头部信息
{
	char szTmp[1024] = { 0 };

	strcat(szTmp, "MAIL FROM:<");
	for(int i = 0; i < listContactor.size(); ++i)
	{
		strcat(szTmp, listContactor[i].c_str());		//当心溢出 
	}
	
	strcat(szTmp, ">\r\n");

	if(!SendNotifyInfo(szTmp, strlen(szTmp)))
	{
		return false;
	}
	//Recv Data;

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
	
	sprintf(szTmp, "--INVT\r\nContent-Type: text/plain;\r\n charset=\"gb2312\"\r\n\r\n%s\r\n\r\n",pszMailTxt);
	
	if(!SendNotifyInfo(szTmp, strlen(szTmp)))
	{
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
