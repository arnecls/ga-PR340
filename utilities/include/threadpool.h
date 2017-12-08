#pragma once

#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <deque>
#include <atomic>
#include "waitgroup.h"

namespace util {

	//! @brief A simple, static threadpool implementation.
	//! Please keep in mind that the number of threads in this pool are static.
	//! Scheduling long running tasks should be avoided.
	class CThreadpool {
	private:
		typedef std::packaged_task<void()> CTask;
		typedef std::shared_ptr<CTask> CTaskPtr;
		typedef std::deque<CTaskPtr> CTaskQueue;

		CTaskQueue mTasks;
		CWaitgroup mWorker;
		std::atomic<bool> mStopRequested;

		std::mutex mTaskLock;
		std::mutex mIdleLock;
		std::condition_variable mIdle;

	public:

		//! @brief Creates a new threadpool with one thread per core.
		CThreadpool();

		//! @brief Creates a new threadpool with a given number of threads.
		//! @param aNumThreads Number of threads to allocate
		CThreadpool(size_t aNumThreads);

		//! @brief Waits for all threads and outstanding tasks to finish.
		~CThreadpool();

		// Disallow copy because it really does not make sense to copy a threadpool.
		CThreadpool(const CThreadpool&) = delete;
		CThreadpool& operator = (const CThreadpool&) = delete;

		//! @brief Schedule a function on the threadpool
		//! @param aFunc Function to run on the threadpool
		//! @return Returns a future to enable waiting for the function to finish
		std::future<void> Run(std::function<void()> aFunc);

	private:

		void Worker();
		void ExecuteNextTask();
	};
}