#pragma once
#include <chrono>
#include <iostream>
#include <string>


class LogDuration 
{
public:
	explicit LogDuration(const std::string& msg = "") : message_(msg + ": "), 
    start_(std::chrono::steady_clock::now()){}

	~LogDuration() 
    {
		const auto finish = std::chrono::steady_clock::now();
		const auto dur = finish - start_;

		std::cerr << message_ << 
			std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << 
			" ms" << std::endl;
	}

private:
    std::string message_;
	std::chrono::steady_clock::time_point start_;
};


#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)


#define TIME_IT(message) \
  LogDuration UNIQ_ID(__LINE__){message};