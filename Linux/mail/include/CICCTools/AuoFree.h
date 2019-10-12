#pragma once
#ifndef __AUTO_FREE__
#define __AUTO_FREE__
#include <stdlib.h>
#include <functional>
using namespace std;

namespace CICCTools
{

	template<typename T = void, typename R = void>
	class AutoFree
	{
	public:
		typedef R(*mmfree) (T*);
		AutoFree(T* ptr, mmfree freefun) :pointer(ptr), myfree(freefun)
		{
			;
		}
		~AutoFree()
		{
			myfree(pointer);
		}
		AutoFree operator =(const AutoFree&) = delete;
		AutoFree(AutoFree&) = delete;

	private:
		T* pointer;
		mmfree myfree;
	};

	template<typename T = void, typename R = void>
	class AutoFreeEx
	{
	public:
		AutoFreeEx(T* ptr, function<R(T*)>  freefun) :pointer(ptr), myfree(freefun)
		{
			;
		}
		~AutoFreeEx()
		{
			myfree(pointer);
		}
		AutoFreeEx operator =(const AutoFreeEx&) = delete;
		AutoFreeEx(AutoFreeEx&) = delete;

	private:
		T* pointer;
		function<R(T*)> myfree;
	};
};

#endif