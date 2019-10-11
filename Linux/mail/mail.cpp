#include "mail.h"


CMail::CMail():m_nPort(25)
{

}

CMail::~CMail()
{

}

bool CMail::OnIntialUpdate(const char* pszConfigFileName)
{
	if(pszConfigFileName == nullptr)
	{
		return false;
	}

	//TODO  获取配置信息

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
	
	return true;
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
