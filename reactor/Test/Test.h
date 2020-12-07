/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxTest.h
 * @brief:xxxTest 
 * @author:xxxTestnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-10-27
 */

#pragma once 


#include <thread> 
#include <stdio.h> 
#include <stdlib.h> 
#include <mutex> 
#include <condition_variable> 



class CTest 
{
	public:
		CTest();
		virtual ~CTest();
public:
	void start();
	
	void stop();
public:
	void threadFunc();
private:
	std::unique_ptr<std::thread>	thread_;
	std::mutex						lock_;
	std::condition_variable			cond_;
	bool							loop_;
};
