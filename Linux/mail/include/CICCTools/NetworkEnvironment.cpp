#include "NetworkEnvironment.h"
#include <thread>
#include <chrono>
#ifdef WIN32
#include <WinSock2.h>
#include <ws2def.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") 
#else
#define SOCKET_ERROR -1
//#include "my_config.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <fcntl.h>
#endif

using namespace CICCTools;
string NetworkEnvironment::m_strIp = "";
string NetworkEnvironment::m_strMac = "";
atomic<int> NetworkEnvironment::m_lock(0);

string& NetworkEnvironment::GetIp()
{
	if (!m_strIp.empty())
	{
		return m_strIp;
	}
	else
	{
		while (m_lock.exchange(1) == 1)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
#ifdef WIN32
		WSAData wsaData;
		if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		{
			return m_strIp;
		}
#endif
		char host_name[255];
		if (gethostname(host_name, sizeof(host_name)) != SOCKET_ERROR)
		{
			struct hostent *phe = gethostbyname(host_name);
			if (phe != 0)
			{
				for (int i = 0; phe->h_addr_list[i] != 0; ++i)
				{
					struct in_addr addr;
					memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
					m_strIp = inet_ntoa(addr) == NULL ? "" : inet_ntoa(addr);
					if (!m_strIp.empty())
					{
						break;
					}
				}
			}
		}

#ifdef WIN32
		WSACleanup();
#endif
		m_lock.exchange(0);
		return m_strIp;
	}
}

string& NetworkEnvironment::GetMac()
{
	if (!m_strMac.empty())
	{
		return m_strMac;
	}
	else
	{
		while (m_lock.exchange(1) == 1)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
#ifdef WIN32
		IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information 
		// for up to 16 NICs
		DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer
		char *mac_addr = (char*)malloc(17);
		DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
			AdapterInfo,                 // [out] buffer to receive data
			&dwBufLen);                  // [in] size of receive data buffer
		//assert(dwStatus == 0);  // Verify return value is 
		// valid, no buffer overflow

		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
		sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
			pAdapterInfo->Address[0], pAdapterInfo->Address[1],
			pAdapterInfo->Address[2], pAdapterInfo->Address[3],
			pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
		m_strMac = mac_addr;
#else
		m_strMac="Un susport";
#endif
	
		m_lock.exchange(0);
		return m_strMac;
	}
}