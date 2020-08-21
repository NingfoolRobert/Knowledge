
#include <string> 
#include <stdlib.h> 
#include <stdio.h> 

#include <unistd.h> 


#define rdtscll(val)  __asm__ __volatile__("rdtsc" : "=A" (val)) 



int main()
{
	long val = 0;
	rdtscll(val);

	printf("%ld\n", val);

	return 0;
}
