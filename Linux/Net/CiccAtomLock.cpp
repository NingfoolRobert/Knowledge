#include "CiccAtomLock.h"
#ifdef WIN32
#include <windows.h>
#define SLEEP Sleep
#else
#include <unistd.h> 
#define SLEEP(x)  usleep(1000*x) 
#endif 



using namespace CICCTools;

AtomLock::AtomLock() :m_Cas(0)
{

};

void AtomLock::Lock()
{
	/*while (m_Cas.exchange(1) == 1)
	{
		//std::this_thread::yield();
		usleep(100);
		//SLEEP(1);//使用Sleep(0) Cpu使用率会飙升		
	}*/
	m_lock.lock();
}

void AtomLock::Unlock()
{
	//m_Cas.exchange(0);
	m_lock.unlock();
}

AtomLockRegion::AtomLockRegion(AtomLock* lock) 
{
	m_lock = lock;
	m_lock->Lock();
}

AtomLockRegion::~AtomLockRegion()
{
	m_lock->Unlock();
}
