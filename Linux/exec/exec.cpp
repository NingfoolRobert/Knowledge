#include "exec.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <signal.h>


using namespace std;



int CExec::Excute(PCMDEXECRES pRes,std::string strScriptName,const std::string strArgs/*=""*/, int nUsec/* = 1000*/, bool bExit/* = false*/, const std::string strShell/* = "/bin/sh"*/,const std::string strEnv/* = "/usr/bin/python2.7"*/)
{
	if(nullptr == pRes)
	{
		//TODO  Log
		return -1;
	}

	
	

	return 0;
}
