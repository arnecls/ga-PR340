#include "threadpool.h"
#include "defer.h"
#include <cassert>

namespace util {

	CThreadpool::CThreadpool() 
		: CThreadpool(std::thread::hardware_concurrency())
	{
	}

	CThreadpool::CThreadpool(size_t aNumThreads)
		: mStopRequested(false) 
	{
		assert(aNumThreads > 0);
		
		// Create detached threads as workers will exit automatically
		mWorker.Add(int(aNumThreads));
		for (size_t i = 0; i < aNumThreads; ++i) {
			std::thread worker(std::bind(&CThreadpool::Worker, this));
			worker.detach();
		}
	}

	CThreadpool::~CThreadpool() {
		// Wake and stop all workers
		mStopRequested = true;
		mIdle.notify_all();
		// Wait for all workers to stop
		mWorker.Wait();
	}

	std::future<void> CThreadpool::Run(std::function<void()> aFunc) {
		GUARD(mTaskLock);

		// As packaged_tasks cannot be copied we need to store them as shared_ptr
		auto task = std::make_shared<CTask>(aFunc);
		mTasks.push_back(task);

		// Wake a worker for the new task and return a "done" handle
		mIdle.notify_one();
		return task->get_future();
	}

	void CThreadpool::ExecuteNextTask() {
		GUARD(mTaskLock);

		// Check again because this time we're threadsafe
		if (mTasks.empty()) {
			return; // ### return, nothing to do ###
		}

		// Execute and remove from list
		(*mTasks.front())();
		mTasks.pop_front();
	}

	void CThreadpool::Worker() {
		defer_obj(mWorker, CWaitgroup::Done);
		std::unique_lock<std::mutex> idleEvent(mIdleLock);

		while (!mStopRequested) {
			while (!mTasks.empty()) {
				ExecuteNextTask();
			}
			// Sleep until woken
			mIdle.wait(idleEvent);
		}
	}
}