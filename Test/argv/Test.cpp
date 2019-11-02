#include "Test.h"
#include "version.h"
#include <string.h> 
#include "marc.h"

char** __argv;
int		__argc;

bool CTest::Execute(const char* pszCommand)
{
	if(__argc > 1 && nullptr != __argv[1])
	{
		if(strncasecmp(__argv[1], "--version",9) == 0)
		{
			printf("version:\t%s\r\nDate:\t \t%s %s\r\n", __SERVICE_VERSION__, __DATE__,__TIME__);
		}
		else if(strncasecmp(__argv[1], "-V",2) == 0)
		{
			printf("version: %s\r\nDate: %s %s\r\n", __SERVICE_VERSION__, __DATE__,__TIME__);
		}
		else 
		{
			printf("-help\r\n\t-V/--version: output the service version\r\n");
		}
		return true;
	}
	
	return true;
}

