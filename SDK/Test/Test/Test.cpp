#include "Test.h"
	
#include <sys/time.h> 
#include <unistd.h> 


CTest::CTest()
{
	m_llSum = 0;
	m_nSend = 0;
	m_nRecv = 0;
}

CTest::~CTest()
{
}

bool CTest::OnInitialUpdate()
{
	CActiveObject::Init(1);

	CActiveObject::SetMaxEventType(10000);
	
	return true;
}


bool CTest::OnEvent(PEVENTHEADER pEvent)
{
	if(nullptr == pEvent)
		return false;

	OnTestEvent(pEvent);

}

void CTest::TestEvent()
{

	TESTEVENT stEvent = { 0 };
	
	stEvent.header.dwLength = sizeof(TESTEVENT) - sizeof(EVENTHEADER);
	stEvent.header.dwType = 1;

	struct timeval tnow;
	gettimeofday(&tnow, NULL);

	//stEvent.tStat = tnow.tv_sec * 1000000LL + tnow.tv_usec;
	stEvent.tSec = tnow.tv_sec;
	stEvent.tUsec = tnow.tv_usec;

	PostEvent((PEVENTHEADER)&stEvent);
	m_nSend++;
}
	
bool CTest::OnTestEvent(PEVENTHEADER pEvent)
{
	m_nRecv++;
	struct timeval tnow;
	gettimeofday(&tnow, NULL);
	PTESTEVENT pTest = (PTESTEVENT)pEvent;
	
	long ll = (tnow.tv_sec - pTest->tSec) * 1000000LL + tnow.tv_usec - pTest->tUsec;
	
	m_llSum += ll;
//	m_llSum += tnow.tv_sec * 1000000LL  + tnow.tv_usec - pTest->tStat;
}
	
void CTest::PrintInfo()
{
	double avg = (double)m_llSum / m_nRecv;
	printf("Send:%ld,Recv:%ld, Sum:%ld, avg:%lf\n", m_nSend.load(), m_nRecv.load(), m_llSum.load(),avg);
}
	
void CTest::TestAlloc()
{
	PTESTEVENT pEvent = new TESTEVENT;
	if(nullptr == pEvent)
	{
		printf("new fail.");
	}
	
	delete pEvent;
	
	pEvent = nullptr;
}
	
void CTest::TestLocker()
{
	m_clsLock.lock();

	static long ll = 0;
	ll++;
	
	m_clsLock.unlock();
}
