#pragma once

//#include <stdio.h>
//
// class singleton has the same goal as all singletons: create one instance of
// // a class on demand, then dish it out as requested.
/********************************************************  
     *  @brief    :  copy from boost/thread/detail/singleton.hpp 
     *  @input    :  
     *  @output   :  
     *  @return   :  
*********************************************************/

template<class T>
class Singleton :private T
{
private:
	Singleton(){}
	~Singleton(){}
public:
	//template<class T>
	static T& GetInstance()
	{
		static Singleton<T> s_oT;
		return (s_oT);
	}

};
