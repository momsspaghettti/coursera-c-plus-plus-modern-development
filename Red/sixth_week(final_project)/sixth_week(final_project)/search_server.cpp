#include "search_server.h"
#include <sstream>
#include <utility>
#include <future>
#include <stdexcept>
#include <algorithm>
#include "iterator_range.h"
#include <iterator>


std::vector<std::string> SplitIntoWords(const std::string& line)
{
    std::istringstream words_input(line);
	return {std::istream_iterator<std::string>(words_input), std::istream_iterator<std::string>() };
}


void SearchServer::UpdateDocumentBase(std::istream& document_input)
{
	words_.clear();
	word_to_ids_.clear();
	id_to_words_ = std::vector<std::unordered_map<std::string_view, int>>(50000);
	word_to_ids_.reserve(10000);

	std::string line;
	size_t id = 0;
	std::unordered_set<std::string> tmp_set;
	tmp_set.reserve(1000);
    while (std::getline(document_input, line))
    {
		tmp_set.clear();
		for (auto& word : SplitIntoWords(line))
		{
			auto iter = words_.insert(word);
			++id_to_words_[id][*iter.first];
			const auto check = tmp_set.insert(*iter.first);
			if (check.second)
				word_to_ids_[*iter.first].push_back(id);
		}
		++id;
    }
}


SearchServer::SearchServer(std::istream& document_input)
{
	UpdateDocumentBase(document_input);
}


void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output) const
{
	std::ios_base::sync_with_stdio(false);
	query_input.tie(nullptr);
	std::vector<int> id_to_hit_count(N);

	std::string line;
    while (std::getline(query_input, line))
    {
		for (size_t i = 0; i < N; ++i)
		{
			id_to_hit_count[i] = 0;
		}

		for (std::string& word : SplitIntoWords(line))
		{
			try
			{
				for (const int& id : word_to_ids_.at(word))
				{
					id_to_hit_count[id] += id_to_words_.at(id).at(word);
				}
			}
			catch (std::out_of_range&)
			{
			}
		}

		std::vector<std::pair<int, int>> tmp;
		tmp.reserve(N);

		for (size_t i = 0; i < N; ++i)
		{
			if (id_to_hit_count.at(i) > 0)
			{
				tmp.push_back({ i, std::move(id_to_hit_count.at(i)) });
			}
		}

		search_results_output << line << ':';

		std::partial_sort(std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.begin() + std::min(5,
			static_cast<int>(tmp.size()))), std::make_move_iterator(tmp.end()),
			[](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
		{
			return std::make_pair(lhs.second, -lhs.first) > std::make_pair(rhs.second, -rhs.first);
		});

		for (auto& [doc_id, hit_count] : Head(tmp, 5))
		{
			search_results_output << " {" << "docid: " << doc_id << ", " << "hitcount: " << hit_count << '}';
		}

		search_results_output << '\n';
    }
}