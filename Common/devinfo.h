#pragma once 


#include "getcpuinfo.h"
#include "getMacAndIP.hpp"
#include "HardDriveSerialNumer.h"

//�����豸��Ϣ
typedef struct tagCurDevInfo
{
	string sMac;
	string sIP;
	string sHdSerialnum;
	string sCPUSerialnum;
}CURDEVINFO, *PCURDEVINFO;

extern CURDEVINFO g_curdevinfo; 

void initfulldevinfo();
