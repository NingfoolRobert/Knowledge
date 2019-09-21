#pragma once

#ifndef CICCATOMLOCK_H
#define CICCATOMLOCK_H

#include <atomic>
#include <mutex>

using namespace std;

namespace CICCTools
{
	class AtomLock
	{
	public:
		AtomLock();
		AtomLock(AtomLock&) = delete;
		AtomLock operator = (AtomLock) = delete;
	private:
		atomic<char> m_Cas;
		mutex m_lock;
	public:
		inline void Lock();

		inline void Unlock();
	};


	class AtomLockRegion
	{
	public:
		AtomLockRegion(AtomLock* lock);
		
		~AtomLockRegion();
		

	private:
		AtomLock *m_lock;
		AtomLockRegion(AtomLockRegion&) = delete;
		AtomLockRegion operator = (AtomLockRegion) = delete;
	};

};

#endif