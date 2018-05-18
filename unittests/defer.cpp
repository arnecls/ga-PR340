#include <gtest/gtest.h>
#include <defer.h>

TEST(DeferTest, main) {
	int result = 0;
	auto func = [&result]() {
		result = 1;
	};

	{
		defer(func);
	}

	EXPECT_EQ(1, result);
}