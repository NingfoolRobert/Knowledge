
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <iostream>

using namespace std;

int Execute(const char* pszCmd)
{
	if(nullptr == pszCmd)
		return -1;
	int ret = system(pszCmd);
	return ret;
}

int Execute(const char* pszCmd, string & strRes, const int nTimeout, bool& bExit, const std::string pszShell)
{
	if(nullptr == pszCmd)
	{
		return -1;
	}
	
	string cmd = pszShell;
	
	//string::size_type pos = strShell.find_last_of('/');
	//if(pos != string::npos)
	{
	//md  = pszShell.substr(pos+1);
	}

	int filedes[2];
	if(pipe(filedes) < 0)
	{
		return -1;
	}
	fcntl(filedes[0], F_SETFL, O_NONBLOCK);

	int nRet =0;
	pid_t pid = fork();
	if(pid < 0)
	{
		return -1;
	}
	if(pid == 0)
	{
		close(filedes[0]);
	}
	if(filedes[1] != STDOUT_FILENO)
	{
		dup2(filedes[1], STDOUT_FILENO);
		dup2(filedes[1], STDERR_FILENO);
		close(filedes[1]);
	}
//	execl(pszShell.c_str(),cmd.c_str());

	char szTmp[256]={0};
	sprintf(szTmp, "%s %s", pszShell.c_str(), pszCmd);
//	system(szTmp);


	execl("/bin/sh","sh","-c", szTmp,(char*)0);
	_exit(127);

	close(filedes[1]);
	

	time_t tBegin = time(NULL);
	
	char buf[1024] = {0};
	while(true)
	{
		memset(buf,0,1024);
		int len = read(filedes[0],buf, 1023);
	
		if(len < 0)
		{
			if(time(NULL) - tBegin < nTimeout && !bExit)
			{
				usleep(1000);
 				continue;
			}
			char Killcmd[128] ={0};
			sprintf(Killcmd, "Kill -KILL %d",pid);
			system(Killcmd);
			len = 0;
			nRet =1;
		}
		if(len == 0)
		{
			if(waitpid(pid, NULL, WNOHANG) == 0)
			{
				continue;
			}
			break;
		}

		strRes +=buf;
	}
	close(filedes[0]);


	return  nRet;
}

int main()
{
	char szTmp[100]={"/usr/bin/python2.7 ./Xtest.py"};
	
//	int nret = Execute(szTmp);
	char szPython[128]={"/usr/bin/python2.7"};
	char szCmd[128]={"./Xtest.py"};

	string str;
	bool bExit = true;
	int nRet = Execute(szCmd,str,1, bExit, szPython);
	std::cout << str << std::endl;

	//execl(szPython,szCmd, NULL);
	return 0;
}
