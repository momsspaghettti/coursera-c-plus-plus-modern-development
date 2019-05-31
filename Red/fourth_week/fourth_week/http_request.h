#pragma once
#include <string_view>


struct HttpRequest 
{
    std::string_view method, uri, protocol;
};