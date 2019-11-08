#include <iostream>
#include "smtp.h"
#include <unistd.h>



int main()
{
	while(true)

	{
		CSmtp smtp;
		smtp.SendEmail();
		usleep(5 * 1000 * 1000);
	}
	return 0;
}
