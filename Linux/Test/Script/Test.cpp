#include <signal.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <string>
#include <iostream>
using namespace std;

string strResult = "";
int ExecuteExit(const string strCmd,string& strRet, const int nTimeout,bool& bExit,const string strShell)
{

	int cPid = 0;
	if(strCmd.empty())
		return -1;

	//signal(SIGCHLD,SIG_IGN);
	string cmd = strShell;
	string::size_type pos = strShell.find_last_of('/');
	if(pos != std::string::npos)
	{
		cmd = strShell.substr(pos+1);
	}
	
	////create pid
	int filedes[2];
	if(pipe(filedes) < 0)
	{
		//printf("[Shell::Execute] pipe create error\n");
		return -1;
	}
	//
	////设置为非阻塞
	fcntl(filedes[0], F_SETFL, O_NONBLOCK);
	////fcntl(filedes[1], F_SETFL, O_NONBLOCK);
	//
	int nRe = 0;
	pid_t pid = fork();
	if(pid < 0)
	{
		//printf("[Shell::Execute] fork error\n");
		return -1;
	}
	//
	////child
//	int cPid = 0;
	else if(pid == 0)
	{
		////printf("child-----------my process id is %d\n",getpid());
		close(filedes[0]);
		if (filedes[1] != STDOUT_FILENO)
		{
			dup2(filedes[1], STDOUT_FILENO);//复制文件句柄用法
			dup2(filedes[1], STDERR_FILENO);
			close(filedes[1]);
		}
		cPid = getpid();
	//
		if(cmd == "perl")
		{
			execl(strShell.c_str(), cmd.c_str(), "-e", strCmd.c_str(), (char *)0);
		}
		//else if(cmd =="Python2.7")
	//	{
	//		execl("/bin/sh","sh","-c",strShell.c_str(),strCmd.c_str(),(char*)0);
	//	}
		else
		{
			execl(strShell.c_str(), cmd.c_str(), "-c", strCmd.c_str(), (char *)0);
		}
	//	printf("[Shell::Execute] child: execl: %s\n", strerror(errno));//转换错误码为对应的错误信息
		_exit(127);
	}
	//
	////parent

	else
	{
		close(filedes[1]);
		time_t btime = time(NULL);
		char buf[10240 + 1] = {0};
		while(true)
		{
			memset(buf,0,sizeof(buf));
			int len = read(filedes[0], buf, sizeof(buf)-1);
		////printf("[Shell::Execute] len =================: %d\n",len);
			if (len < 0)
			{
				if (time(NULL) - btime < nTimeout && !bExit)
				{
					usleep(1000);
					continue;
				}
		////printf("[Shell::Execute] parent: kill child pid = %d for timeout %d s\n",pid,nTimeout);
				char cmd[100] = {0};
				sprintf(cmd,"kill -9 %d",pid);
				system(cmd);
				len = 0;
				nRe = 1;
			}
		//
		////recv finish or timeout -> break
			if(len == 0)
			{
				if(waitpid(pid, NULL, WNOHANG) ==  0)
				{
//					if(errno == EINTR)
						continue;
///					break;
				}
			////printf("[Shell::Execute] parent: child pid = %d exit\n",pid);
				break;
			}
	////printf("[Shell::Execute] parent: %s\n",buf);
		strRet += buf;
//		std::cout << strRet << std::endl;
		strResult = strRet;
	}
	
	}
	close(filedes[0]);
	//
	return nRe;
}


int main ()
{
	string strShell ="/bin/sh";
	string strCmd = "/usr/bin/python2.7  ./Xtest.py";
	string strRes = "";
	bool bExit = false;
	int nRet = ExecuteExit(strCmd, strRes, 1, bExit, strShell);
	std::cout << nRet << std::endl;
	printf("%s\n",strRes.c_str() );
	std::cout << strResult << std::endl;
//	while(1)
	{
		usleep(10);
	}
	return nRet;
}
