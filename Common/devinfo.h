#pragma once 


#include "getcpuinfo.h"
#include "getMacAndIP.hpp"
#include "HardDriveSerialNumer.h"

//本机设备信息
typedef struct tagCurDevInfo
{
	string sMac;
	string sIP;
	string sHdSerialnum;
	string sCPUSerialnum;
}CURDEVINFO, *PCURDEVINFO;

extern CURDEVINFO g_curdevinfo; 

void initfulldevinfo();
