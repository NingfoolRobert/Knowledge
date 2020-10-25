/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxEventLoopThread.h
 * @brief:xxxEventLoopThread 
 * @author:xxxEventLoopThreadnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-10-25
 */


#pragma once 

#include <stdio.h> 
#include <stdlib.h> 
#include <thread> 
#include <mutex> 
#include <condition_variable> 

class EventLoop; 
class EventLoopThread 
{
public:
	typedef  std::function<void(EventLoop*)>  ThreadInitCallback; 
	EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(), const std::string& name = "");
	~EventLoopThread();
public:
	EventLoop* startLoop();
	void stopLoop();
private:
	void threadFunc();
private:
	EventLoop*							loop_;
	bool								exiting_;
	std::unique_ptr<std::thread>		thread_;
	std::mutex							lock_;
	std::condition_variable				cond_;
	ThreadInitCallback					callback_;
};

