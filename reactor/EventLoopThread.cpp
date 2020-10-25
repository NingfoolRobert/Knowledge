#include "EventLoopThread.h"
#include "EventLoop.h"



	
EventLoopThread::EventLoopThread(const ThreadInitCallback& cb/* = ThreadInitCallback()*/, 
								const std::string& name/* = ""*/):loop_(nullptr),exiting_(false),callback_(cb)
{
}
								
EventLoopThread::~EventLoopThread()
{
	exiting_ = true;
	if(loop_ == nullptr)
	{
		loop_->quit();
		thread_->join();
	}
}

	
EventLoop* EventLoopThread::startLoop()
{
	thread_.reset(new std::thread(std::bind(&EventLoopThread::threadFunc, this)));
	{
		std::unique_lock<std::mutex> locker(lock_);
		while(loop_ == nullptr)
		{
			cond_.wait(locker);
		}
	}

	return loop_;
}
	
void EventLoopThread::stopLoop()
{
	if(loop_)
		loop_->quit();
	
	thread_->join();
}
	
void EventLoopThread::threadFunc()
{
	EventLoop loop; 
	if(callback_)
	{
		callback_(&loop);
	}
	{
		std::unique_lock<std::mutex> locker(lock_);
		loop_ = &loop;
		cond_.notify_all();
	}
	loop.loop();
	loop_ = nullptr;
}
