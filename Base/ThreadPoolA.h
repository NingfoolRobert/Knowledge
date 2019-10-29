#pragma once

#include <vector>
#include <queue>
#include <thread>
//#include <iostream>
#include <stdexcept>
#include <condition_variable>
#include <memory>
#include <functional>
#include <mutex>
//#include <unique_lock>


typedef std::function<void(void)> Task;

class CThreadPool
{
public:
	CThreadPool(int num = 1);
	virtual ~CThreadPool();
	
public:
	bool AddTask(Task task);
private:
	static void* Worker(void*arg);
	void Run();
private:
	std::vector<std::thread> work_thread;
	std::queue<Task> tasks_queue;
	
	std::mutex queue_mutex;
	std::condition_variable condition;
	bool m_bstop;
};

extern class CThreadPool* g_pThreadPool;
