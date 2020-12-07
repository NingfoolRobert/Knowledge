


#include "Test.h"
#include <unistd.h>

int main() 
{
	CTest tst;
	
	printf("befor start....\n");
	tst.start();
	printf("after start ...");
	

	usleep( 100 * 1000 * 1000);

	printf("stop before....\n");
	tst.stop();
	printf("stop after....\n");

	
}
