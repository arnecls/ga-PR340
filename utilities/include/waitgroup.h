#pragma once

#include <mutex>
#include <atomic>

namespace util {

	//! @brief A simple implementation of the waitgroup concept.
	class CWaitgroup {
	private:
		struct SState {
			std::mutex mIdleLock;
			std::condition_variable mIdle;
			std::atomic<int> mCount;
		};
		std::shared_ptr<SState> mState; // to support copies

	public:

		//! @brief Initialize the waitgroup.
		CWaitgroup();

		//! @brief Add a given number of wait events.
		//! @param aCount Number of wait events to add.
		void Add(int aCount);

		//! @brief Add a single wait events.
		void Inc();

		//! @brief Mark one wait event as done.
		void Done();

		//! @brief Wait for all events to be done.
		void Wait();

		//! @brief Returns the current state of the wait function.
		//! @return Return true when Wait() would wait.
		bool TryWait();
	};
}