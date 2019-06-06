#pragma once
#include <chrono>
#include <iostream>
#include <sstream>


struct TotalDuration {
    std::string message;
    std::chrono::steady_clock::duration value;
	
    explicit TotalDuration(const std::string& msg = "")
		: message(msg + ": ")
		, value(0)
	{
	}
	
    ~TotalDuration() {
        std::ostringstream os;
		os << message
			<< std::chrono::duration_cast<std::chrono::milliseconds>(value).count()
			<< " ms" << std::endl;
		std::cerr << os.str();
	}
};


class AddDuration 
{
public:
	explicit AddDuration(std::chrono::steady_clock::duration& dest)
		: add_to(dest)
		, start(std::chrono::steady_clock::now())
	{
	}
	
    explicit AddDuration(TotalDuration& dest)
		: AddDuration(dest.value)
	{
	}
	
    ~AddDuration() {
		add_to += std::chrono::steady_clock::now() - start;
	}
private:
    std::chrono::steady_clock::duration& add_to;
    std::chrono::steady_clock::time_point start;
};


#define ADD_DURATION(value) \
AddDuration UNIQ_ID(__LINE__){value};