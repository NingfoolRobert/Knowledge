//Ĭ��֧�ֶ��߳���Ӳ������������ȷ��ֻ�ǵ��߳���ӣ������SetMultiThreadWrite(false)����������Ч��

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
				AtomLockRegion lock(&_lockEnqueue);	//ȷ�����߳����ʱ��_ptok��Ψһռ�ã������ʹ�����ݱ��ƻ�
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
				AtomLockRegion lock(&_lockEnqueue);	//ȷ�����߳����ʱ��_ptok��Ψһռ�ã������ʹ�����ݱ��ƻ�
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
			//	//AtomLockRegion lock(&_lockDequeue);	//ȷ�����̳߳���ʱ��_ctok��Ψһռ�ã������ʹ�����ݱ��ƻ�
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
			//	//AtomLockRegion lock(&_lockDequeue);	//ȷ�����̳߳���ʱ��_ctok��Ψһռ�ã������ʹ�����ݱ��ƻ�
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
			//	//AtomLockRegion lock(&_lockDequeue);	//ȷ�����̳߳���ʱ��_ctok��Ψһռ�ã������ʹ�����ݱ��ƻ�
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
			//	//AtomLockRegion lock(&_lockDequeue);	//ȷ�����̳߳���ʱ��_ctok��Ψһռ�ã������ʹ�����ݱ��ƻ�
			//	return _queue.try_dequeue_bulk(_ctok, itemFirst, max);
			//}
			//else
			{
				return _queue.wait_dequeue_bulk(itemFirst, max);
			}
		}

		//��ֹ�����ȴ����Ա�Wait_Dequeue���������أ������ĵ���Ҳ����������ʧ��
		void StopWait()
		{
			//_queue.stopWait();
		}

		//���¿�ʼ�������ݣ�
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
