#include "HighPresionTimer.h"


using namespace std;
using namespace std::chrono;
using namespace CICCTools;

#ifdef MYWIN32
HighPresionTimer::HighPresionTimer()
{
	LARGE_INTEGER  large_interger;
	QueryPerformanceFrequency(&large_interger);
	frequency = large_interger.QuadPart;
	QueryPerformanceCounter(&large_interger);
	starttime = large_interger.QuadPart;
}
void HighPresionTimer::Reset()
{
	LARGE_INTEGER  large_interger;
	QueryPerformanceCounter(&large_interger);
	starttime = large_interger.QuadPart;	
}


double HighPresionTimer::GetInterval()
{

	LARGE_INTEGER  large_interger;
	QueryPerformanceCounter(&large_interger);

	long long endtime = large_interger.QuadPart;

	return  (endtime - starttime) * 1000.0 / frequency;
}
double HighPresionTimer::operator-(const HighPresionTimer &t) const
{
	return (starttime - t.starttime) * 1000.0 / frequency;
}
#else 
HighPresionTimer::HighPresionTimer() :m_begin(high_resolution_clock::now())
{
}

void HighPresionTimer::Reset()
{
	m_begin = high_resolution_clock::now();
}

double HighPresionTimer::GetInterval()
{
	typedef duration<double, std::ratio<1, 1000>> myms;
	return duration_cast<nanoseconds>(high_resolution_clock::now() - m_begin).count()/1000000.0;


}
double HighPresionTimer::operator-(const HighPresionTimer &t) const
{
	typedef duration<double, std::ratio<1, 1000>> myms;
	return duration_cast<myms> (m_begin - t.m_begin).count();
}



#endif




