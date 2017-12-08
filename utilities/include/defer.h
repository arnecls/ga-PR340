#pragma once

#include <functional>
#include "macros.h"

namespace util {
	//! @brief Helper class to execute calls at end of scope.
	//! You should use the defer and defer_obj macros instead of using this
	//! class directly.
	class CDeferred {
	private:
		std::function<void()> mFunction;
	public:
		//! @brief Stores the given function
		//! @brief aFunc Function to call at end of lifetime
		inline CDeferred(std::function<void()> aFunc) : mFunction(aFunc) {}

		//! @brief Calls the stored function
		inline ~CDeferred() { mFunction(); }
	};
}

//! @brief Execute the given function at the end of the current scope.
//! @param dFunc Non-member function to execute
#define defer(dFunc) util::CDeferred CONCAT(d,__LINE__)(dFunc)

//! @brief Execute the given member function at the end of the current scope.
//! @param dObj Object instance holding the member function to call
//! @param dFunc Member function to execute
#define defer_obj(dObj, dFunc) util::CDeferred CONCAT(d,__LINE__)(std::bind(&dFunc, dObj))
