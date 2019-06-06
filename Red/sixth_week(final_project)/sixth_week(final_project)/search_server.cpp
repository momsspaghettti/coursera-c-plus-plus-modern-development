#include "search_server.h"
#include <sstream>
#include <utility>
#include <future>
#include <stdexcept>
#include <algorithm>
#include "iterator_range.h"
#include <iterator>


std::vector<std::string> SplitIntoWords(std::string&& line)
{
    std::istringstream words_input(std::move(line));
	return {std::make_move_iterator(std::istream_iterator<std::string>(words_input)), 
	    std::make_move_iterator(std::istream_iterator<std::string>()) };
}


void SearchServer::UpdateDocumentBase(std::istream& document_input)
{
	word_to_ids_.clear();
	id_to_words_ = std::vector<std::unordered_map<std::string, int>>(50000);
	word_to_ids_.reserve(10000);
	docs_count = 0;

	std::string line;
	size_t id = 0;
	std::unordered_set<std::string> tmp_set;
	tmp_set.reserve(1000);
    while (std::getline(document_input, line))
    {
		tmp_set.clear();
		for (auto& word : SplitIntoWords(std::move(line)))
		{
			++id_to_words_[id][word];
			const auto check = tmp_set.insert(word);
			if (check.second)
				word_to_ids_[word].push_back(id);
		}
		++id;
    }

	docs_count = id;
}


SearchServer::SearchServer(std::istream& document_input)
{
	UpdateDocumentBase(document_input);
}


void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output) const
{
	std::ios_base::sync_with_stdio(false);
	query_input.tie(nullptr);
	std::vector<int> id_to_hit_count(docs_count);
	std::vector<std::pair<int, int>> tmp;
	tmp.reserve(docs_count);

	std::string line;
    while (std::getline(query_input, line))
    {
		id_to_hit_count.assign(docs_count, 0);

		for (std::string& word : SplitIntoWords(std::move(line)))
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

		tmp.clear();

		for (size_t i = 0; i < docs_count; ++i)
		{
			if (id_to_hit_count.at(i) > 0)
			{
				tmp.emplace_back(std::make_pair(i, id_to_hit_count.at(i)));
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