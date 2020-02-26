
#pragma once 
#include <unistd.h> 

#include <stdlib.h> 
#include <stdio.h> 

#include <mutex> 
#include <condition_variable> 
#include <time.h> 
#include <sys/time.h> 
#include <thread> 

#include "PtrCycle.h"



typedef struct stTestInfo 
{
	int nID;
	struct timeval tmStart;
}TESTINFO, *PTESTINFO;


class CTest 
{
	public:
		CTest()
		{
			m_lMax = 0;
			m_lMin = 100000;
			m_lSum = 0;
			m_nCount = 0;
			m_dAvg = 0;
		}
		~CTest()
		{

		}
		
	public:
		
		bool Init()
		{
			for(int i = 0; i < 2; ++i)
			{
				std::thread tr1(&CTest::ActiveThreadFunc, this);
				tr1.detach();
			}
			m_listInfo.SetExpandLen(10000);
		}
		void ActiveThreadFunc()
		{
			while(true)
			{
				std::unique_lock<std::mutex> locker(m_clsLock);
				m_cond.wait(locker, [&]()->bool{ return !m_listInfo.IsEmpty();});
				PTESTINFO pInfo = m_listInfo.Get();
				while(pInfo)
				{
					struct timeval tmNow; 
					gettimeofday(&tmNow, nullptr);
					long ldiff = (tmNow.tv_sec - pInfo->tmStart.tv_sec) * 1000000 + tmNow.tv_usec - pInfo->tmStart.tv_usec;
					m_lSum += ldiff;
					m_lMax = m_lMax > ldiff ? m_lMax : ldiff;
					m_lMin = m_lMin > ldiff ? ldiff : m_lMin;
					m_nCount++;
					pInfo = nullptr;
					pInfo = m_listInfo.Get();
				}
				
			}
		}
		bool Add(PTESTINFO pInfo)
		{
			if(nullptr == pInfo)
				return true;
			struct timeval tmNow;
			gettimeofday(&tmNow, nullptr);
			pInfo->tmStart.tv_sec = tmNow.tv_sec;
			pInfo->tmStart.tv_usec = tmNow.tv_usec;
			std::unique_lock<std::mutex> locker(m_clsLock);			
			m_listInfo.Add(pInfo);
			m_cond.notify_one();
		}
	private:
		std::mutex				m_clsLock;
		std::condition_variable	m_cond;
		CPtrCycle<TESTINFO>		m_listInfo;
	public:
		long					m_lMax;
		long					m_lMin;
		long					m_lSum;
		int						m_nCount;
		double					m_dAvg;
};
