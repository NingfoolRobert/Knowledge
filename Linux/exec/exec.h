#pragma  once

#include <iostream>

#include <string>

typedef struct stCmdExecResult
{
	bool			bTimeout;
	std::string		StdOut;
	int				nerrno;

	stCmdExecResult()
	{
		bTimeout = false;
		StdOut ="";
		nerrno  = 0;
	}
}CMDEXECRES, *PCMDEXECRES;


class CExec
{
public:
	CExec() = default;
	~CExec() = default;
public:
	int Excute(PCMDEXECRES pRes,std::string strScriptName,const std::string strArgs="", int nUsec = 1000, bool bExit = false, const std::string strShell = "/bin/sh",const std::string strEnv = "/usr/bin/python2.7");
};
