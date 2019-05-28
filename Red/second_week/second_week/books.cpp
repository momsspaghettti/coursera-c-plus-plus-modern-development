#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <map>
#include <unordered_map>


class ReadingManager 
{
public:
	void Read(int user_id, int page_count)
	{
		if (user_to_page_.count(user_id) == 0)
		{
			user_to_page_[user_id] = page_count;
			page_to_users_[page_count].insert(user_id);
		}
        else
        {
			const int old_page = user_to_page_.at(user_id);
			user_to_page_[user_id] = page_count;

			page_to_users_.at(old_page).erase(user_id);
			page_to_users_[page_count].insert(user_id);

            if (page_to_users_.at(old_page).empty())
            {
				page_to_users_.erase(old_page);
            }
        }
	}

	double Cheer(int user_id) const
	{
		const auto count_ = user_to_page_.count(user_id);

	    if (user_to_page_.size() == 1 && count_ == 1)
	    {
			return 1;
	    }

        if (count_ == 0)
        {
			return 0;
        }

		const int current_page = user_to_page_.at(user_id);

		auto pos = page_to_users_.find(current_page);

        if (pos == page_to_users_.begin())
        {
			return 0;
        }

		--pos;

		double res = 0;

        do
        {
			res += pos->second.size();

            if (pos == page_to_users_.begin())
                break;

			--pos;
        }
        while (true);

		return res / (user_to_page_.size() - 1);
	}

private:
	std::unordered_map<int, int> user_to_page_;
	std::map<int, std::unordered_set<int>> page_to_users_;
};


/*
 * int main() 
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
	std::string query_type;
	int user_id;

    std::cin >> query_count;
    
    for (int query_id = 0; query_id < query_count; ++query_id) 
	{
        std::cin >> query_type;
        std::cin >> user_id;
        
        if (query_type == "READ") 
		{
            int page_count;
            std::cin >> page_count;
            manager.Read(user_id, page_count);
		} 
        else if (query_type == "CHEER") 
		{
            std::cout << std::setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}

  return 0;
}
 */