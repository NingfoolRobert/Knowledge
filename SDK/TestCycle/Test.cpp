

#include "Test.h"




int main()
{
	CTest Test;
	Test.Init();

	for(int i = 0; i < 100000; ++i)
	{
		PTESTINFO pInfo = new TESTINFO;
		if(nullptr == pInfo) continue;
		Test.Add(pInfo);
	}

	usleep(10 * 1000 * 1000);

	Test.m_dAvg = (double)Test.m_lSum / (double) Test.m_nCount;
	printf("Sum: %ld, Count: %d, Max: %ld, Min:%ld, avg: %lf\n",Test.m_lSum, Test.m_nCount, Test.m_lMax, Test.m_lMin, Test.m_dAvg);

	return 0;
}
