#include "ThreadPoolA.h"

class CThreadPool* g_pThreadPool = nullptr;

CThreadPool::CThreadPool(int num/* = 1*/):m_bstop(false)
{
	if(num <=0)
		throw std::exception();

	for(int i = 0; i < num; ++i)
	{
		work_thread.emplace_back(CThreadPool::Worker,this);
	}
	g_pThreadPool = this;
}

CThreadPool::~CThreadPool()
{
	std::unique_lock<std::mutex> lock(queue_mutex);
	m_bstop = true;
	condition.notify_all();
	for(auto &ww: work_thread)
		ww.join();
}



bool CThreadPool::AddTask(Task task)
{
	queue_mutex.lock();
	tasks_queue.push(task);
	queue_mutex.unlock();
	condition.notify_one();
	return true;
}


void* CThreadPool::Worker(void*arg)
{
	CThreadPool* pool = (CThreadPool*) arg;
	pool->Run();
	return pool;
}
void CThreadPool::Run()
{
	while(!m_bstop)
	{
		std::unique_lock<std::mutex> lock(this->queue_mutex);
		this->condition.wait(lock, [this]{return !this->tasks_queue.empty();});
		if(this->tasks_queue.empty())
		{
			continue;
		}
		else 
		{
			Task task = tasks_queue.front();
			tasks_queue.pop();
			task();
		}
	}
}
