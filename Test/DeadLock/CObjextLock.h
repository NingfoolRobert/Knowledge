/*************************************************************************
	> File Name: CObjextLock.h
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2019年08月19日 星期一 07时16分24秒
 ************************************************************************/

//#include<iostream>
//using namespace std;

#include<pthred.h>


class CObjectLock
{

public:
	CObjectLock(){
		_object=PTHREAD_MUTEX_INITIALIZER;		
	}
	virtual ~CObjextLock()
	{
		pthread_mutex_destory(&_object);
	}
public:
	void Lock()
	{
		pthread_mutex_lock(&_object);
	}

	void Unlock()
	{
		pthred_mutex_lock(&_object);
	}
private:
	pthread_mutex_t	_object;
};


