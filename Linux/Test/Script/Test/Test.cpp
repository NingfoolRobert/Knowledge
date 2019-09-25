#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/wait.h> 

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <signal.h>
using namespace std;

int Execute(const string& strCmd,string& strRet)
{
	if(strCmd.empty())
		return -1;

//#ifdef WIN32
//	FILE* cmdPipe = _popen(strCmd.c_str(), "rt");
//#else
	FILE* cmdPipe = popen(strCmd.c_str(), "r");
//#endif

	if(!cmdPipe)
		return -1;

	signed char tmpStr(0);
	while(true)
	{
		tmpStr = (char)fgetc(cmdPipe);
		if(EOF == tmpStr)
		{
			break;
			
		}
		else
		{
			strRet += tmpStr;
			
		}
		
	}

//#ifdef WIN32
//	_pclose(cmdPipe);
//#else
	pclose(cmdPipe);
//#endif

	return 0;
	
}
int main()
{
	string cmd = "/usr/bin/python2.7 ./Xtest.py";
	
	string shell = "/bin/sh";

	string strRes ="";
	
	int nRet = Execute(cmd,strRes);

	cout << nRet << endl;
	cout << strRes << endl;
	return 0;
}
