

#include "zmqPub.h"

#include <stdio.h>
#include <stdlib.h>


int main()
{
	CZMQPub* pPub = new CZMQPub;
	
	pPub->Init("tcp://127.0.0.1:30303");
	char szTopic[32] = { "1234332" };
	CBuffer* pBuffer = new CBuffer;
	if(nullptr == pBuffer)
	{
		return -1;
	}
	if(!pBuffer->Append(szTopic, strlen(szTopic)))
	{
		printf("Append fail.");
		return -1;
	}

	while(1)
	{
		pPub->PostMsg(szTopic, pBuffer);
		usleep(1000 * 1000);
	}


	return 0;

}
