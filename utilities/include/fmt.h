#pragma once
#include <iostream>

namespace util {

	template <typename T>
	void println(T aValue) {
		std::cout << aValue << std::endl;
	}

	template <typename T, typename... MORE>
	inline void println(T aValue, MORE... more) {
		std::cout << aValue;
		println(more);
	}
}