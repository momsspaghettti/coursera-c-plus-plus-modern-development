#pragma once
#include "iterator_range.h"
#include <string>
#include <sstream>
#include <vector>


template <typename Container>
std::string Join(char c, const Container& cont) 
{
    std::ostringstream os;
  
    for (const auto& item : Head(cont, cont.size() - 1)) 
	{
        os << item << c;
    }
  
    os << *rbegin(cont);
    return os.str();
}


std::string_view Strip(std::string_view s);


std::vector<std::string_view> SplitBy(std::string_view s, char sep);