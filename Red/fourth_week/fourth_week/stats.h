#pragma once
#include "http_request.h"
#include <map>
#include <array>


class Stats 
{
public:
	Stats();

    void AddMethod(std::string_view method);
  
    void AddUri(std::string_view uri);
    
    const std::map<std::string_view, int>& GetMethodStats() const;
    
    const std::map<std::string_view, int>& GetUriStats() const;

private:
	std::map<std::string_view, int> methods_to_count_;
	std::map<std::string_view, int> uris_to_count_;

	inline const static std::array<std::string, 5> methods_ = { "GET", "POST", 
	    "PUT", "DELETE", "UNKNOWN" };
	inline const static std::array<std::string, 6> uris_ = { "/", "/order",
	    "/product", "/basket", "/help", "unknown" };
};


HttpRequest ParseRequest(std::string_view line);


void TestStats();