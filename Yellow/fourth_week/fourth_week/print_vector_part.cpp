#include "print_vector_part.h"
#include <algorithm>
#include <iostream>


void PrintVectorPart(const std::vector<int>& numbers)
{
	IntIter int_iter = std::find_if(numbers.begin(), numbers.end(),
		[](const int& num) {return num < 0; });

    while (int_iter != numbers.begin())
    {
		--int_iter;
        std::cout << *int_iter << " ";
    }
}
