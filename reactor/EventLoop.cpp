#include "EventLoop.h"



	
EventLoop::EventLoop()
{

}
	
EventLoop:: ~EventLoop()
{

}


void EventLoop::loop()
{
	looping_ = true;
	quit_  = false;
	
	while(!quit_)
	{
		//TODO  timer  
		
		//IO handle 
		//
		//
		//
		doPendingFunctors();
	}
	
}
	
void EventLoop::quit()
{
	//TODO  
}
	
bool EventLoop::queueInLoop(const Functor& cb)
{
	{
		std::unique_lock<std::mutex> locker(lock_);
		pendingFunctor_.push_back(cb);
	}
	// 外部线程调用时， 唤醒线程 
	if(!isInLoopThread() || callingPendFunctors_)
	{
		//TODO wake thread 
	}

	return true;
}

	
bool EventLoop::runInLoop(const Functor& cb)
{
	if(isInLoopThread())
	{
		cb();
	}
	else 
	{
		queueInLoop(cb);
	}
	return true;
}
	
void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	{
		std::unique_lock<std::mutex>	locker(lock_);
		functors.swap(pendingFunctor_);
	}

	for(auto i = 0u; i < functors.size(); ++i)
		functors[i]();
}
