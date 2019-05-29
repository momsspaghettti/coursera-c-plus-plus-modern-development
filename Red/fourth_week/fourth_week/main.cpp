#include <iostream>
#include <list>
#include <unordered_map>
#include <stdexcept>


void Sportsmen()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::list<int> sportsmen;
	std::unordered_map<int, std::list<int>::iterator> on_field;
	int n, sportsman, pos;

	std::cin >> n;

    for (int i = 0; i < n; ++i)
    {
		std::cin >> sportsman >> pos;

		try
		{
			const auto iter = on_field.at(pos);
			sportsmen.insert(iter, sportsman);

			on_field[sportsman] = std::prev(iter);
		}
		catch (std::out_of_range&)
		{
			sportsmen.insert(sportsmen.end(), sportsman);

			on_field[sportsman] = std::prev(sportsmen.end());
		}
    }

    for (const int& item : sportsmen)
    {
		std::cout << item << ' ';
    }
}


int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	Sportsmen();

    return 0;
}