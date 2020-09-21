#include "devinfo.h"
#include "Log.h"


CURDEVINFO g_curdevinfo;

void initfulldevinfo()
{
	getMacAndIP(g_curdevinfo.sMac, g_curdevinfo.sIP);
	if (g_curdevinfo.sMac.size() == 0)
		g_curdevinfo.sMac = "NA";
	if (g_curdevinfo.sIP.size() == 0)
		g_curdevinfo.sIP = "NA";

	char szCpuInfo[128] = { 0 };
	get_cpuId(szCpuInfo);
	g_curdevinfo.sCPUSerialnum = szCpuInfo;
	if (g_curdevinfo.sCPUSerialnum.size() == 0)
		g_curdevinfo.sCPUSerialnum = "NA";
	getHDSerialnum(g_curdevinfo.sHdSerialnum);
	if (g_curdevinfo.sHdSerialnum.size() == 0)
		g_curdevinfo.sHdSerialnum = "NA";
#if _TEST_ 
	using namespace  UtilTool;
	MYLOG.WriteLog("[INFO] CPU:%s, HD:%s, IP:%s, MAC:%s", g_curdevinfo.sCPUSerialnum.c_str(),
		g_curdevinfo.sHdSerialnum.c_str(),
		g_curdevinfo.sIP.c_str(),
		g_curdevinfo.sMac.c_str());
#endif
}