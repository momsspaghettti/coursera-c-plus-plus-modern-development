#include "parse.h"


std::string_view Strip(std::string_view s) 
{
	size_t pos = s.find_last_not_of(' ');
	s.remove_suffix(s.size() - pos - 1);

	pos = s.find_first_not_of(' ');
	s.remove_prefix(pos);

	return s;
}


std::vector<std::string_view> SplitBy(std::string_view s, char sep) 
{
    std::vector<std::string_view> result;
	result.reserve(s.size() / 2);

	size_t pos = s.find_first_not_of(sep);
	s.remove_prefix(pos);

    while (pos != s.npos)
    {
		pos = s.find_first_of(sep);

		result.push_back(s.substr(0, pos));

        if (pos == s.npos)
            break;

		s.remove_prefix(pos);
		pos = s.find_first_not_of(sep);

		if (pos == s.npos)
			break;

		s.remove_prefix(pos);
    }
	result.shrink_to_fit();

    return result;
}