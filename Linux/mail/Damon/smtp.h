
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h> 


struct Base64Date6
{
	unsigned int d4 : 6;
	unsigned int d3 : 6;
	unsigned int d2 : 6;
	unsigned int d1 : 6;
					
};

class CSmtp
{
	public:
		CSmtp();
		~CSmtp();
	public:


		void  SendEmail();
		bool  OpenSocket();
		int   SendHead();
		bool  SendBody();
		bool  SendEnd();
	protected:

		int		LogOn();
		
		int		SendMsg(const char* pszBuf, int nLen,bool bWaitRecv = false, char* pszRecvBuf = nullptr, int nLength = 0);

		
	void EncodeBase64(char *dbuf, char *buf128, int len);

	char ConvertToBase64(char uc);

	private:
		int		m_fd;
		char	m_szEHLO[32];
		char	m_szAccount[32];
		char	m_szPassWd[32];
		char	m_szHostIP[32];
		char	m_szFromEmail[64];
		char	m_szAuth[32];
};
