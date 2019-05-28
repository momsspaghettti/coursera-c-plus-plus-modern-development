#include <iostream>
#include <string>
#include <map>
#include <set>


class RouteManager 
{
public:
	void AddRoute(int start, int finish)
    {
		routes_[start].insert(finish);
		routes_[finish].insert(start);
    }

	int FindNearestFinish(int start, int finish) const
    {
		if (start == finish)
		{
			return 0;
		}

        if (routes_.count(start) == 0)
        {
			return std::abs(start - finish);
        }

		auto lower = routes_.at(start).lower_bound(finish);

		int result = 2 * 1'000'000'000 + 1;

        if (lower != routes_.at(start).end())
        {
			result = std::abs(*lower - finish);
        }

        if (std::abs(start - finish) < result)
        {
			result = std::abs(start - finish);
        }

        if (lower != routes_.at(start).begin())
        {
			const int tmp = *--lower;
            if (std::abs(tmp - finish) < result)
            {
				result = std::abs(tmp - finish);
            }
        }

		return result;
    }

private:
	std::map<int, std::set<int>> routes_;
};


/*
 * int main() 
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

    RouteManager routes;

    int query_count;
	int start, finish;
	std::string query_type;

    std::cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id)
	{
		std::cin >> query_type;
		std::cin >> start >> finish;

		if (query_type == "ADD")
		{
			routes.AddRoute(start, finish);
		}
		else if (query_type == "GO")
		{
			std::cout << routes.FindNearestFinish(start, finish) << "\n";
		}
	}
    
    return 0;
}
 */