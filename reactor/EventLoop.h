/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxEventLoop.h
 * @brief:xxxEventLoop 
 * @author:xxxEventLoopnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-10-25
 */

#pragma once 

#include <stdio.h> 
#include <stdlib.h> 
#include <functional>
#include <mutex>
#include <vector> 
#include <thread> 



typedef void (*PASYNCFUNC)();

class EventLoop 
{
public:
	typedef std::function<void()> Functor;
	EventLoop();
	virtual ~EventLoop();
public:
	void	loop();
	
	// stop loop
	void	quit();

	bool	queueInLoop(const Functor& cb);

	bool	runInLoop(const Functor& cb);

	bool	isInLoopThread() { return threadid_ == std::this_thread::get_id(); }

protected:
	void	doPendingFunctors();
private:

	std::mutex						lock_;
	std::vector<Functor>			pendingFunctor_ ;
private: 
	const  std::thread::id			threadid_;
private:
	bool							looping_;
	bool							quit_;
	bool							callingPendFunctors_;
};
