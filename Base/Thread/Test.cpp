
#include <iostream>

#include "Thread.h"
#include <unistd.h> 


using namespace std;

void* TestThread(void* pThread)
{
	for(int i = 0; i < 10; ++i)
		cout << " 10" << endl;
	
	usleep(1000000);
	return nullptr;
}

int main ()
{

	CThread tr1;
	tr1.Init(TestThread);
	
	if(!	tr1.Run( 1 ))
		cout << " fail" << endl;
	else 
		cout << "success"  << endl;
	getchar();
	return 0;
}
