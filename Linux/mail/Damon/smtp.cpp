#include "smtp.h"

#include <string.h>
#include <memory>


CSmtp::CSmtp()
{
	strcpy(m_szHostIP, "192.168.20.43");
	strcpy(m_szEHLO, "EHLO CICCGWGroup\r\n");
	strcpy(m_szAccount, "CICC\\it_support_app_sende");
	strcpy(m_szPassWd, "setupandindex");
	strcpy(m_szFromEmail, "it_support_app_sende@cicc.com.cn");
	strcpy(m_szAuth, "AUTH LOGIN\r\n");

}
CSmtp::~CSmtp()
{

}

void CSmtp::SendEmail()
{
	do 
	{
		if(!OpenSocket())
		{
			break;
			//	return false;
		}
		if(!LogOn())
		{
			break;
			//return false;
		}
		if(!SendHead())
			break;
		if(!SendBody())
			break;
		if(!SendEnd())
			break;
	}while(0);
	
	if(close(m_fd) < 0)
	{
		printf("close socket fail. errno:%d", errno);
	}
}
bool CSmtp::OpenSocket()
{
	m_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(m_fd <= 0 )
	{
		printf("create socket fail");
		return  false;
	}

	struct hostent *host = gethostbyname(m_szHostIP);
	if(nullptr == host)
	{
		return false;
	}
	char szTmp[32] = { 0 };;
	if(host->h_addrtype == AF_INET)
	{
		inet_ntop(host->h_addrtype, host->h_addr_list[0],szTmp, sizeof(szTmp));
	}
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(szTmp); 

	if(connect(m_fd, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
	{
		printf("connect fail.");
		return false;
	}
	
	return true;
}

int	CSmtp::LogOn()
{
	char szRecv[256] = { 0 };
	if(SendMsg(m_szEHLO, strlen(m_szEHLO), true, szRecv, sizeof(szRecv)) < 0)
	{
		printf("Send EHLO fail.");
		return 0;
	}
	if(strncmp(szRecv, "250") != 0)
	{
		printf(" Recv EHLO Error:%s", szRecv);
		return 0;
	}

	memset(szRecv, 0, sizeof(szRecv));
	int nRet =SendMsg(m_szAuth, strlen(m_szEHLO), true, szRecv, sizeof(szRecv));
	if(nRet < 0)
	{
		printf("Send Auth fail.");
		return 0;
	}
	else if(nRet == 0)
	{
		printf("Recv Length is 0.");
		return false;
	}
	if(strstr(szRecv,"334") == nullptr)
	{
		printf("Auth fail Error: %s", szRecv);
	}


	
	//User Log on 
	char szUserName[64] = { 0 };
	char szPasswd[64] = { 0 };
	EncodeBase64(szUserName, m_szAccount, strlen(m_szAccount));
	printf("EncodeBase64 UserName: %s", szUserName);	
	strcat(szUserName, "\r\n");

	nRet = SendMsg(szUserName, strlen(szUserName));
	if(nRet < 0)
	{
		printf("Send Msg fail.");
		return false;
	}
	EncodeBase64(szPasswd, m_szPassWd, strlen(m_szPassWd));
	printf("EncodeBase64 Passwd: %s", szPasswd);
	strcat(szPasswd, "\r\n");
	memset(szRecv, 0, sizeof(szRecv));
	nRet = SendMsg(szPasswd, strlen(szUserName), true, szRecv, sizeof(szRecv));
	if(nRet < 0)
	{
		printf("Send Msg fail.");
		return false;
	}
	else if(nRet == 0)
	{
		printf("Log on fail.");
		return false;
	}
	
	if(strstr(szRecv, "250") == nullptr || nullptr == strstr(szRecv, "334"))
	{
		printf("Log On Error: %s", szRecv);
		return false;
	}
	return false;

}
int CSmtp::SendHead()
{
	char szRecv[256] = {0};
	char szTmp[256] = { 0 };
	strcpy(szTmp, "MAIL FROM: <it_support_app_sende@cicc.com.cn>\r\n");
	int nRet = SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv));
	if(nRet < 0)
	{
		printf("Send HEADER MAIL FROM Msg fail.");
		return 0;
	}
	else if(nRet == 0)
	{
		printf("Recv Msg fail.");
		return 0;
	}

	memset(szRecv, 0, sizeof(szRecv));
	strcpy(szTmp, "RCPT TO: <Beifei.Ning@cicc.com.cn>\r\n");
	nRet = SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv));
	if(nRet < 0)
	{
		printf("Send HEADER RCPT TO Msg fail.");
		return 0;
	}
	else if(nRet == 0)
	{
		printf("Recv Msg fail.");
		return 0;
	}
	

}

bool CSmtp::SendBody()
{
	
	char szTmp[256] = { "DATA\r\n" };
	char szRecv[256] = { 0 };
	
	int nRet = SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv));
	if(nRet < 0)
	{
		printf("Send HEADER DATA HEADER Msg fail.");
		return 0;
	}
	else if(nRet == 0)
	{
		printf("Recv Msg fail.");
		return 0;
	}

	strcpy(szTmp, "From: \"IT\"<it_support_app_sende@cicc.com.cn>\r\nTo: <Beifei.Ning@cicc.com.cn>\r\nSubject:ninhao\r\n\r\nTestOne.\r\n.\r\n");
	
	memset(szRecv, 0, sizeof(szRecv));
	nRet = SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv));
	if(nRet < 0)
	{
		printf("Send HEADER DATA HEADER Msg fail.");
		return 0;
	}
	else if(nRet == 0)
	{
		printf("Recv Msg fail.");
		return 0;
	}
	printf("Send Result: %s", szRecv);
	return true;
}

bool CSmtp::SendEnd()
{
	char szTmp[256] = {0};
	strcpy(szTmp, "QUIT\r\n");
	char szRecv[256] = {0};
	int nRet = SendMsg(szTmp, strlen(szTmp), true, szRecv, sizeof(szRecv));
	if(nRet < 0)
	{
		printf("Send HEADER DATA HEADER Msg fail.");
		return 0;
	}
	else if(nRet == 0)
	{
		printf("Recv Msg fail.");
		return 0;
	}
	  
	printf("Quit result :%s", szRecv);
	
	return true;
}

int	CSmtp::SendMsg(const char* pszBuf, int nLen,bool bWaitRecv/* = false*/, char* pszRecvBuf/* = nullptr*/, int nLength/* = 0*/)
{
	int nSendLen = send(m_fd, pszBuf, nLen, 0);
	if(nSendLen <= 0)
	{
		printf("Send Error : %d",errno);
		return -1;
	}

	if(!bWaitRecv)
	{
		return 0;
	}
	
	int nRecvLen = recv(m_fd, pszRecvBuf, nLength, 0);
	if(nRecvLen <= 0)
	{
		printf("Send Error : %d" , errno);
		return 0;
	}
	
	return nRecvLen;

}


void CSmtp::EncodeBase64(char *dbuf, char *buf128, int len)
{
	struct Base64Date6 *ddd = NULL;
	int           i = 0;
	char          buf[256] = { 0  };
	char          *tmp = NULL;
	char          cc = '\0';
	memset(buf, 0, 256);
	strcpy(buf,  buf128);
	
	for (i = 1; i <= len / 3; i++)
	{
		tmp = buf + (i - 1) * 3;
		
		cc = tmp[2];
	
		tmp[2] = tmp[0];
		
	
		tmp[0] = cc;
		
		ddd = (struct Base64Date6 *)tmp;
		
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		
		dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
		
		dbuf[(i - 1) * 4 + 3] = ConvertToBase64((unsigned int)ddd->d4);
	}
	
	if (len % 3 == 1)
	{
		tmp = buf + (i - 1) * 3;
		
		cc = tmp[2];
		
		tmp[2] = tmp[0];
		
		tmp[0] = cc;
		ddd = (struct Base64Date6 *)tmp;
		
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		
		dbuf[(i - 1) * 4 + 2] = '=';
		
		dbuf[(i - 1) * 4 + 3] = '=';
	}
	
	if (len % 3 == 2)
	{
		tmp = buf + (i - 1) * 3;
		
		cc = tmp[2];
		
		tmp[2] = tmp[0];
	
		tmp[0] = cc;
		
		ddd = (struct Base64Date6 *)tmp;
		
		dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
		
		dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
		
		dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
		
		dbuf[(i - 1) * 4 + 3] = '=';
		
	}
	
	return;
}
char CSmtp::ConvertToBase64(char uc)
{
	if (uc < 26)
	{
		return 'A' + uc;
	}
	
	if (uc < 52)
	{
		return 'a' + (uc - 26);
	}
	
	if (uc < 62)
	{
		return '0' + (uc - 52);
	}
	
	if (uc == 62)
	{
		return '+';
	}
	return '/';
}
