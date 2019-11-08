#include "czmq.h"

#include <iostream>

int main()
{
	//zsock_t sock = zsock_new(ZMQ_SUB);
	
	zsock_t* subcribel = zsock_new_sub("tcp://localhost:5556", "10001");
	
	char buffer[256]={0};
	int nCount = 10;
	while(nCount--)
	{
		memset(buffer, 0, sizeof(buffer));

		int nSize = zmq_recv(subcribel, buffer, 255, 0);
		if(nSize < -1)
		{
			std::cout << " fail..." << std::endl;
			return -1;
		}
		
		std::cout << buffer << std::endl;
	}
	zsock_destroy(&subcribel);
	return 0;
}
