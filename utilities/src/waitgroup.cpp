#include "waitgroup.h"
#include <cassert>

namespace util {
	CWaitgroup::CWaitgroup()
	{
		mState = std::make_shared<SState>();
		mState->mCount = 0;
	}

	void CWaitgroup::Add(int aCount) {
		mState->mCount += aCount;
	}

	void CWaitgroup::Inc() {
		++mState->mCount;
	}

	void CWaitgroup::Done() {
		assert(mState->mCount > 0);
		--mState->mCount;
		if (mState->mCount == 0) {
			mState->mIdle.notify_all();
		}
	}

	void CWaitgroup::Wait() {
		std::unique_lock<std::mutex> idle(mState->mIdleLock);
		if (TryWait()) {
			mState->mIdle.wait(idle);
		}
	}

	bool CWaitgroup::TryWait() {
		return mState->mCount > 0;
	}
}
