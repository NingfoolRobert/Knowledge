//默认支持多线程入队操作，如果程序确认只是单线程入队，请调用SetMultiThreadWrite(false)，能提高入队效率

#pragma once
#ifndef CONCURRENTQUEUENEW_H
#define CONCURRENTQUEUENEW_H

#include "concurrentqueue.h"
//#include "blockingconcurrentqueue.h"
//#
#include "ConCurrentQueue.h"
#include "CiccAtomLock.h"


namespace CICCTools {

	using namespace moodycamel;

	template<typename T> class ConcurrentQueueNew
	{
	public:
		ConcurrentQueueNew(size_t capacity = 6 * ConcurrentQueueDefaultTraits::BLOCK_SIZE,bool multiWrite=false) : _queue(capacity), _ptok(_queue), _multi_thread_write(multiWrite)//, _ctok(_queue)
		{
			//_multi_thread_read = true;
			// _multi_thread_write = false;
		}

		~ConcurrentQueueNew()
		{

		}

		void SetMultiThreadWrite(bool multiWrite)
		{
			_multi_thread_write = multiWrite;
		}

		//void SetMultiThreadRead(bool multiRead)
		//{
		//	_multi_thread_read = multiRead;
		//}

		bool Enqueue(const T& data)
		{
			if (_multi_thread_write)
			{
				AtomLockRegion lock(&_lockEnqueue);	//确保多线程入队时对_ptok的唯一占用，否则会使得数据被破坏
				return _queue.enqueue(_ptok, data);
			}
			else
			{
				return _queue.enqueue(data);
			}
		}

		template<typename It>
		bool Enqueue_bulk(It itemFirst, size_t count)
		{
			if (_multi_thread_write)
			{
				AtomLockRegion lock(&_lockEnqueue);	//确保多线程入队时对_ptok的唯一占用，否则会使得数据被破坏
				return _queue.enqueue_bulk(_ptok, itemFirst, count);
			}
			else
			{
				return _queue.enqueue_bulk(itemFirst, count);
			}
		}

		bool Dequeue(T& data)
		{
			//if (_multi_thread_write)
			//{
			//	//AtomLockRegion lock(&_lockDequeue);	//确保多线程出队时对_ctok的唯一占用，否则会使得数据被破坏
			//	return _queue.try_dequeue(_ctok, data);
			//}
			//else
			{
				return _queue.try_dequeue(data);
			}
		}

		template<typename It>
		size_t Dequeue_bulk(It itemFirst, size_t max)
		{
			//if (_multi_thread_write)
			//{
			//	//AtomLockRegion lock(&_lockDequeue);	//确保多线程出队时对_ctok的唯一占用，否则会使得数据被破坏
			//	return _queue.try_dequeue_bulk(_ctok, itemFirst, max);
			//}
			//else
			{
				return _queue.try_dequeue_bulk(itemFirst, max);
			}
		}

		bool Wait_Dequeue(T& data)
		{
			//if (_multi_thread_write)
			//{
			//	//AtomLockRegion lock(&_lockDequeue);	//确保多线程出队时对_ctok的唯一占用，否则会使得数据被破坏
			//	return _queue.try_dequeue(_ctok, data);
			//}
			//else
			{
				return _queue.wait_dequeue(data);
			}
		}

		template<typename It>
		size_t Wait_Dequeue_bulk(It itemFirst, size_t max)
		{
			//if (_multi_thread_write)
			//{
			//	//AtomLockRegion lock(&_lockDequeue);	//确保多线程出队时对_ctok的唯一占用，否则会使得数据被破坏
			//	return _queue.try_dequeue_bulk(_ctok, itemFirst, max);
			//}
			//else
			{
				return _queue.wait_dequeue_bulk(itemFirst, max);
			}
		}

		//终止阻塞等待，以便Wait_Dequeue能正常返回，后续的调用也将立即返回失败
		void StopWait()
		{
			//_queue.stopWait();
		}

		//重新开始接收数据，
		void Resume()
		{
			_queue.resume();
		}

	private:
		BlockingConcurrentQueue<T> _queue;
		ProducerToken _ptok;
		//ConsumerToken _ctok;

		AtomLock _lockEnqueue;
		//CLockAtom _lockDequeue;

		bool _multi_thread_write;
		//bool _multi_thread_read;
	};
};
#endif
