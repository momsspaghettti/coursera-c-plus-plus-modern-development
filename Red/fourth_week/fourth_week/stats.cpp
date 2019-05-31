#include "stats.h"
#include <algorithm>


HttpRequest ParseRequest(std::string_view line)
{
	auto pos = line.find_first_not_of(' ');
	line.remove_prefix(pos);

	pos = line.find(' ');
	const std::string_view method = std::string_view(line.data(), pos);
	line.remove_prefix(pos + 1);

	pos = line.find(' ');
	const std::string_view uri = std::string_view(line.data(), pos);
	line.remove_prefix(pos + 1);

	const std::string_view protocol = std::string_view(line.data());

	return { method, uri, protocol };
}


Stats::Stats()
{
    for (size_t i = 0; i < 5; ++i)
    {
		methods_to_count_[methods_.at(i)] = 0;
		uris_to_count_[uris_.at(i)] = 0;
    }

	uris_to_count_[uris_.at(5)] = 0;
}


void Stats::AddMethod(std::string_view method)
{
	const auto find_iter = std::find(methods_.begin(), 
		methods_.end(), method);

    if (find_iter == methods_.end())
    {
		++methods_to_count_[*std::prev(methods_.end())];
    }
    else
    {
		++methods_to_count_[methods_.at(find_iter - methods_.begin())];
    }
}


void Stats::AddUri(std::string_view uri)
{
	const auto find_iter = std::find(uris_.begin(), 
		uris_.end(), uri);

    if (find_iter == uris_.end())
    {
		++uris_to_count_[*std::prev(uris_.end())];
    }
    else
    {
		++uris_to_count_[uris_.at(find_iter - uris_.begin())];
    }
}


const std::map<std::string_view, int>& Stats::GetMethodStats() const
{
	return methods_to_count_;
}


const std::map<std::string_view, int>& Stats::GetUriStats() const
{
	return uris_to_count_;
}