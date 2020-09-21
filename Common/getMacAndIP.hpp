#ifndef		_GET_MAC_IP_HPP_
#define		_GET_MAC_IP_HPP_


#include <string>
#include <stdio.h>      //该头文件声明了输入输出流函数
#include <stdlib.h>     //该头文件定义了一些通用函数
#include <WinSock2.h>
#include<IPHlpApi.h>
#pragma  comment(lib, "IPHlpApi.lib")
using namespace std;

inline bool getMacAndIP(std::string& sMac, std::string& sIP)
{
	char szMac[64] = { 0 };
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
		PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//记录网卡数量
	int netCardNum = 0;
	//记录每张网卡上的IP地址数量
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		// 可能有多网卡,默认取第一个网卡信息
	 	sprintf(szMac, "%02X-%02X-%02X-%02X-%02X-%02X",
			pIpAdapterInfo->Address[0],
			pIpAdapterInfo->Address[1],
			pIpAdapterInfo->Address[2],
			pIpAdapterInfo->Address[3],
			pIpAdapterInfo->Address[4],
			pIpAdapterInfo->Address[5]
		);
		sMac = szMac;
	//可能网卡有多IP,因此通过循环去判断
	IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
	//默认取第一个
	sIP = pIpAddrString->IpAddress.String;
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = nullptr;
	}

	return true;
}


#endif