#include "UnitTestsFramework.h"
#include "http_request.h"
#include "stats.h"
#include <map>
#include <string_view>


Stats ServeRequests(std::istream& input) 
{
    Stats result;
    for (std::string line; std::getline(input, line); ) 
	{
        const HttpRequest req = ParseRequest(line);
        result.AddUri(req.uri);
        result.AddMethod(req.method);
    }
    return result;
}


void TestBasic() 
{
    const std::string input =
        R"(GET / HTTP/1.1
        POST /order HTTP/1.1
        POST /product HTTP/1.1
        POST /product HTTP/1.1
        POST /product HTTP/1.1
        GET /order HTTP/1.1
        PUT /product HTTP/1.1
        GET /basket HTTP/1.1
        DELETE /product HTTP/1.1
        GET / HTTP/1.1
        GET / HTTP/1.1
        GET /help HTTP/1.1
        GET /upyachka HTTP/1.1
        GET /unexpected HTTP/1.1
        HEAD / HTTP/1.1)";

    const std::map<std::string_view, int> expected_method_count = {
        {"GET", 8},
        {"PUT", 1},
        {"POST", 4},
        {"DELETE", 1},
        {"UNKNOWN", 1},};
  
    const std::map<std::string_view, int> expected_url_count = {
        {"/", 4},
        {"/order", 2},
        {"/product", 5},
        {"/basket", 1},
        {"/help", 1},
        {"unknown", 2},};

    std::istringstream is(input);
    const Stats stats = ServeRequests(is);

    ASSERT_EQUAL(stats.GetMethodStats(), expected_method_count);
    ASSERT_EQUAL(stats.GetUriStats(), expected_url_count);
}


void TestAbsentParts() 
{
    // Методы GetMethodStats и GetUriStats должны возвращать словари
    // с полным набором ключей, даже если какой-то из них не встречался

    const std::map<std::string_view, int> expected_method_count = {
        {"GET", 0},
        {"PUT", 0},
        {"POST", 0},
        {"DELETE", 0},
        {"UNKNOWN", 0},};
  
    const std::map<std::string_view, int> expected_url_count = {
        {"/", 0},
        {"/order", 0},
        {"/product", 0},
        {"/basket", 0},
        {"/help", 0},
        {"unknown", 0},};
  
    const Stats default_constructed;

    ASSERT_EQUAL(default_constructed.GetMethodStats(), expected_method_count);
    ASSERT_EQUAL(default_constructed.GetUriStats(), expected_url_count);
}


void TestStats() 
{
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
    RUN_TEST(tr, TestAbsentParts);
}
