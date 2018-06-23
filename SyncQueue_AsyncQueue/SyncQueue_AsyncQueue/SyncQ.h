#pragma once

#include <mutex>
#include <condition_variable>
#include <cassert>
#include <type_traits>
#include <atomic>
#include <deque>

namespace moon
{
	template <class T, class TContainer = std::deque<T>, size_t max_size = 50>
	class sync_queue
	{
	public:
		sync_queue(): m_bexit(false){}
	private:
		sync_queue(const sync_queue& test) {}
		sync_queue& operator=(const sync_queue& test){}

	public:
		
		void push_back(const T& t)
		{
			std::unique_lock<std::mutex>lck(m_mutex);
			m_notfull.wait(lck, [this] { return m_bexit || (m_dequeue.size() < max_size); });
			m_dequeue.push_back(t);
		}
		template<class _Tdata>
		void emplace_back(_Tdata& v)
		{
			std::unique_lock<std::mutex>lck(m_mutex);
			m_notfull.wait(lck, [this] { return m_bexit || (m_dequeue.size() < max_size); });
			m_dequeue.emplace_back(std::forward<_Tdata>(v));
		}
		size_t size()
		{
			std::unique_lock<std::mutex>lck(m_mutex);
			return m_dequeue.size();
		}
		TContainer Move()
		{
			std::unique_lock<std::mutex>lck(m_mutex);
			auto tmp = std::move(m_dequeue);
			m_notfull.notify_one();
			return std::move(tmp);
		}
		void exit()
		{
			m_bexit = true;
		}

	private:
		std::mutex					m_mutex;
		std::condition_variable		m_notfull;
		TContainer					m_dequeue;
		std::atomic_bool			m_bexit;
	};
}