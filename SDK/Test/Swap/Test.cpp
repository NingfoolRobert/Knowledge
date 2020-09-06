

#include "buffer.h"


int main()
{

	CBuffer stBuf1;
	CBuffer stBuf2;
	stBuf1.Set(10, 100, 1000);
	stBuf2.Set(1, 11, 111);

	printf("Swap Befor, 1: %d, %d, %d\n", stBuf1.GetBufLen(), stBuf1.GetCap(), stBuf1.GetHeaderLen());
	printf("Swap Befor, 2: %d, %d, %d\n", stBuf2.GetBufLen(), stBuf2.GetCap(), stBuf2.GetHeaderLen());
	
	stBuf1.exchange(stBuf2);


	printf("Swap After, 1: %d, %d, %d\n", stBuf1.GetBufLen(), stBuf1.GetCap(), stBuf1.GetHeaderLen());
	printf("Swap After, 2: %d, %d, %d\n", stBuf2.GetBufLen(), stBuf2.GetCap(), stBuf2.GetHeaderLen());
	

	return 0;
}
