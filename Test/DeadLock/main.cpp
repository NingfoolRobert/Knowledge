/*************************************************************************
	> File Name: main.cpp
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2019年08月19日 星期一 07时09分22秒
 ************************************************************************/

#include<iostream>
#include <pthread.h>
#include <unistd.h>
#include  "CAtuoLock.h"



using namespace std;



class Request
{

public:
	void process();	//attribute_


private:
	pthread_mutex_t	mutex;
}
