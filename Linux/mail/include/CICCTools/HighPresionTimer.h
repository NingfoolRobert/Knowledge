#pragma once

#ifndef HIGHPRESIONTIMER_H
#define HIGHPRESIONTIMER_H




#ifdef WIN32
#include <windows.h>
#define MYWIN32
#endif
#include <stdio.h>
#include <time.h>
#include <chrono>

namespace CICCTools
{
	class HighPresionTimer
	{
	public:
		HighPresionTimer();
		void Reset();
		double GetInterval();
		double operator -(const HighPresionTimer &t) const;
	private:
#ifdef MYWIN32
		long long frequency;
		long long starttime;
#else


		std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;

#endif

	};


#endif


};




