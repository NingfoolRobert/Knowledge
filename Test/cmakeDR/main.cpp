#include <iostream>
#include <stdio.h>
#include <stdlib.h>




int main()
{
#ifdef DEBUG 
	printf("Debug Mode.\n");
#else 
	printf("Release Mode.\n");
#endif 
	while(true)
	{
		;
	}
	
	return 0;
}
