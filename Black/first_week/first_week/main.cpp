#include <iostream>
#include <vector>


int main()
{
	std::vector<int> v{ 8 };
	v.reserve(2);

	const size_t i = 1;

	std::cout << v[i];

	return 0;
}