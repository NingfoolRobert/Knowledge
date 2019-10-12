#pragma once

#ifndef CICCDATETIME_H
#define CICCDATETIME_H


#ifdef WIN32
#include <windows.h>
#else
#include<sys/time.h>
#endif 
#include <time.h>



#include <chrono>
#include <string>
using namespace std;

namespace CICCTools
{
	class CICCDateTime
	{
	private:
		CICCDateTime();
	public:
		static string GetCurrentSystemDate(string seperator)
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm* ptm = localtime(&t);
			char date[32];
			sprintf(date, "%4d%s%02d%s%02d", (int)ptm->tm_year + 1900, seperator.c_str(),(int)ptm->tm_mon + 1, seperator.c_str(), (int)ptm->tm_mday);
			return string(date);
		}
		static string GetCurrentSystemDate()
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm* ptm = localtime(&t);
			char date[32];
			sprintf(date, "%4d%02d%02d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday);
			return string(date);
		}	

		static string GetPreviousDate(int i=1)
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() - i* std::chrono::hours(24));
			struct tm* ptm = localtime(&t);
			char date[32];
			sprintf(date, "%4d%02d%02d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday);
			return string(date);
		}

		static string GetCurrentSystemTime()
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm* ptm = localtime(&t);
			char time[32];

#ifdef WIN32
			SYSTEMTIME systime;
			GetSystemTime(&systime);
			WORD time_ms = systime.wMilliseconds;
			sprintf(time, "%02d%02d%02d%03d", (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)time_ms);
#else
			struct timeval now_ms;
			auto time_ms = gettimeofday(&now_ms, 0);
			sprintf(time, "%02d%02d%02d%03d", (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)now_ms.tv_usec / 1000);
#endif // WIN32
			
			return string(time);
		}

		static string GetCurrentSystemPartTime()//返回小时和分钟
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm* ptm = localtime(&t);
			char time[32];
			sprintf(time, "%02d%02d", (int)ptm->tm_hour, (int)ptm->tm_min);
			return string(time);
		}
		static string GetCurrentSystemPartTimeSecond()//返回小时、分钟、秒hhmmss
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm* ptm = localtime(&t);
			char time[32];
			sprintf(time, "%02d%02d%02d", (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
			return string(time);
		}
		static string GetCurrentSystemDateTimeLogFormat()
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm* ptm = localtime(&t);
			char time[32];

#ifdef WIN32
			SYSTEMTIME systime;
			GetSystemTime(&systime);
			WORD time_ms = systime.wMilliseconds;
			sprintf(time, "[%4d-%02d-%02d %02d:%02d:%02d.%03d]", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)time_ms);
#else
			struct timeval now_ms;
			auto time_ms = gettimeofday(&now_ms, 0);
			sprintf(time, "[%4d-%02d-%02d %02d:%02d:%02d.%03d]", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)now_ms.tv_usec / 1000);
#endif // WIN32
			
			return string(time);
		}
		static string GetCurrentSystemDateTime()
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm* ptm = localtime(&t);
			char time[32];

#ifdef WIN32
			SYSTEMTIME systime;
			GetSystemTime(&systime);
			WORD time_ms = systime.wMilliseconds;
			sprintf(time, "%4d%02d%02d%02d%02d%02d%03d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)time_ms);
#else
			struct timeval now_ms;
			auto time_ms = gettimeofday(&now_ms, 0);
			sprintf(time, "%4d%02d%02d%02d%02d%02d%03d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)now_ms.tv_usec / 1000);
#endif // WIN32
			
			return string(time);
		}
		static int64_t GetLinuxStamp()
		{
			std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
			//std::time_t timestamp = tmp.count();
			//std::time_t timestamp = std::chrono::system_clock::to_time_t(tp);  

			return tmp.count();
		}
		static string GetCurrentSystemDateTimSplitBy()
		{
			auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm* ptm = localtime(&t);
			char time[32];

#ifdef WIN32
			SYSTEMTIME systime;
			GetSystemTime(&systime);
			WORD time_ms = systime.wMilliseconds;
			sprintf(time, "[%4d-%02d-%02d %02d:%02d:%02d.%03d]", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)time_ms);
#else
			struct timeval now_ms;
			auto time_ms = gettimeofday(&now_ms, 0);
			sprintf(time, "%4d-%02d-%02d %02d:%02d:%02d.%03d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)now_ms.tv_usec / 1000);
#endif // WIN32

			return string(time);
		}
	};
};

#endif


