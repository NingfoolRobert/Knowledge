#include <stdio.h>
#include <stdlib.h>



class CUserObject
{
public:
	CUserObject();
	virtual ~CUserObject();
public:
	virtual bool OnMsg(const char* pszMsg);


private:
	
};
