#ifndef		_GET_MAC_IP_HPP_
#define		_GET_MAC_IP_HPP_


#include <string>
#include <stdio.h>      //��ͷ�ļ��������������������
#include <stdlib.h>     //��ͷ�ļ�������һЩͨ�ú���
#include <WinSock2.h>
#include<IPHlpApi.h>
#pragma  comment(lib, "IPHlpApi.lib")
using namespace std;

inline bool getMacAndIP(std::string& sMac, std::string& sIP)
{
	char szMac[64] = { 0 };
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
		PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//��¼��������
	int netCardNum = 0;
	//��¼ÿ�������ϵ�IP��ַ����
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		// �����ж�����,Ĭ��ȡ��һ��������Ϣ
	 	sprintf(szMac, "%02X-%02X-%02X-%02X-%02X-%02X",
			pIpAdapterInfo->Address[0],
			pIpAdapterInfo->Address[1],
			pIpAdapterInfo->Address[2],
			pIpAdapterInfo->Address[3],
			pIpAdapterInfo->Address[4],
			pIpAdapterInfo->Address[5]
		);
		sMac = szMac;
	//���������ж�IP,���ͨ��ѭ��ȥ�ж�
	IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
	//Ĭ��ȡ��һ��
	sIP = pIpAddrString->IpAddress.String;
	}
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = nullptr;
	}

	return true;
}


#endif