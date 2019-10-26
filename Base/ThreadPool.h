
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include "Singleton.h"


class ThreadPool
{
public:
	ThreadPool();
	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
		->std::future<typename std::result_of<F(Args...)>::type>;
	~ThreadPool();
public:
	bool Init(int cnThread);
	bool enqueue(std::function<void()>& task);
private:
	// need to keep track of threads so we can join them
	std::vector< std::thread > workers;
	// the task queue
	std::queue< std::function<void()> > tasks;

	// synchronization
	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop;

//public:
//	static ThreadPool* pool;
//	static void Initial(size_t);
//	static void Release();


	//template<class F, class... Args>
	//static auto Enqueue(F&& f, Args&&... args)
	//	->std::future<typename std::result_of<F(Args...)>::type>;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool():stop(false)
{

}
inline bool ThreadPool::Init(int cnThread = 10)
//	: stop(false)
{
	for (int i = 0; i <  cnThread; ++i)
		workers.emplace_back(
		[this]
	{
		for (;;)
		{
			std::function<void()> task;

			{
		 		std::unique_lock<std::mutex> lock(this->queue_mutex);
				this->condition.wait(lock,
					[this]{ return this->stop || !this->tasks.empty(); });
				if (this->stop && this->tasks.empty())
					return;
				task = std::move(this->tasks.front());
				this->tasks.pop();
			}

			task();
		}
	}
	);
	return true;
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
	-> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()> >(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex);

		// don't allow enqueueing after stopping the pool
		if (stop)
			throw std::runtime_error("enqueue on stopped ThreadPool");

		tasks.emplace([task](){ (*task)(); });
	}
	condition.notify_one();
	return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	for (std::thread &worker : workers)
		worker.join();
}

//by yyk
//template<class F, class... Args>
//inline auto ThreadPool::Enqueue(F&& f, Args&&... args)
//	->std::future<typename std::result_of<F(Args...)>::type>
//{
//	return pool->enqueue(std::forward<F>(f), std::forward<Args>(args)...);
//}

bool ThreadPool::enqueue(std::function<void()>& task)
{
	std::unique_lock<std::mutex> lock(queue_mutex);
	tasks.emplace([task](){ (task)(); });
	return true;
}


#define THREADPOOL  Singleton<ThreadPool>::GetInstacne()

#define THREADINIT(nThreadCount)			Singleton<ThreadPool>::GetInstacne().Init(nThreadCount)

#define PostEvent(task)	Singleton<ThreadPool>::GetInstance.enqueue(task)

#define ASYNC_HANDLE(f,...) Singleton<ThreadPool>::GetInstance.enqueue(f,Args)





#endif
