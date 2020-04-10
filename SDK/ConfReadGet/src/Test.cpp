#include "Configure.h"



int main()
{
	CIniConfigure  stIni;
	stIni.Init("./Test.ini");

	int val = stIni.GetIniInt("WIN", "LogLevel", 1);


	printf("%d\n", val);

	char szTmp[20] = { 0 };
	stIni.GetIniString("Stage", "Addr", "", szTmp);
	printf("%s\n", szTmp);


	printf("%lf", stIni.GetIniDouble("WIN", "price", 0.0));
	return 0;

	
}
