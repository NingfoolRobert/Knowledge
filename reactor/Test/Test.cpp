#include "Test.h"
#include <unistd.h> 



CTest::CTest():loop_(false)
{
	
}
		
CTest::~CTest()
{

}



void CTest::start()
{
	thread_.reset(new std::thread(&CTest::threadFunc, this));
	{
		std::unique_lock<std::mutex> locker(lock_);
		while(!loop_ )
		{
			cond_.wait(locker);
		}
	}
}
	
void  CTest::stop()
{
	if(loop_)
		 loop_ = false;
	thread_->join();
}
	
void CTest::threadFunc()
{
	{
		std::unique_lock<std::mutex> locker(lock_);
		loop_ = true;
		cond_.notify_all();
	}

	while(loop_)
	{
		printf("loop info ....\n");
		usleep(3* 1000 * 1000);
	}
}
