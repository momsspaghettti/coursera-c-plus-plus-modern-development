#pragma once
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>


class Combiner
{
public:
	Combiner() = delete;

	Combiner(int64_t, int64_t);

	void PerformSum();

	void PrintResult(std::ostream&);

private:
	const int64_t first_;
	const int64_t second_;

	std::optional<int64_t> result_ = std::nullopt;

	[[nodiscard]] bool sum_is_overflow() const;

	friend void TestReadNumbers();
	friend void TestSumLittleNumbers();
	friend void TestSumIsOverflow();
};


Combiner InitCombiner(std::istream&);


void TestSum();