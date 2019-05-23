#include "UnitTestsFramework.h"
#include "max_element_if.h"
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <numeric>
#include <iterator>


void TestUniqueMax() 
{
    const auto IsEven = [](int x)
    {
        return x % 2 == 0;
    };

    const std::list<int> hill{2, 4, 8, 9, 6, 4, 2};
    auto max_iterator = hill.begin();
    advance(max_iterator, 2);

    std::vector<int> numbers(10);
    iota(numbers.begin(), numbers.end(), 1);

    /*
    Мы не используем AssertEqual, потому что для итераторов
    отсутствует перегрузка оператора вывода в поток ostream.
    Разыменование здесь также недопустимо, так как оно может повлечь
    неопределенное поведение, если функция max_element_if, к примеру,
    вернула итератор, указывающий на конец контейнера.
    */

	ASSERT(max_element_if(numbers.begin(), numbers.end(), IsEven) == --numbers.end());

    ASSERT(max_element_if(hill.begin(), hill.end(), IsEven) == max_iterator);
}


void TestSeveralMax() 
{
    struct IsCapitalized 
    {
    bool operator()(const std::string &s) const
    {
        return !s.empty() && isupper(s.front());
    }
    };

    const std::forward_list<std::string> text{"One", "two", "Three", "One", 
        "Two", "Three", "one", "Two", "three"};
    
    auto max_iterator = text.begin();
    advance(max_iterator, 4);

    ASSERT(max_element_if(text.begin(), text.end(), IsCapitalized()) == max_iterator);
}


void TestNoMax() 
{
    const std::vector<int> empty;
    const std::string str = "Non-empty string";

    const auto AlwaysTrue = [](int)
    {
        return true;
    };

    ASSERT(max_element_if(empty.begin(), empty.end(), AlwaysTrue) == empty.end());

    const auto AlwaysFalse = [](char)
    {
        return false;
    };

    ASSERT(max_element_if(str.begin(), str.end(), AlwaysFalse) == str.end());
}


void TestAllMaxElemetIfTests()
{
    TestRunner tr;

	RUN_TEST(tr, TestUniqueMax);
	RUN_TEST(tr, TestSeveralMax);
	RUN_TEST(tr, TestNoMax);
}