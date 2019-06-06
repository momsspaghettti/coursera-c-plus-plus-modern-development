#include "search_server.h"
#include "iterator_range.h"
#include <numeric>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <future>


std::vector<std::string> SplitIntoWords(const std::string& line)
{
    std::istringstream wordsInput(line);
	return {std::make_move_iterator(std::istream_iterator<std::string>(wordsInput)), 
	    std::make_move_iterator(std::istream_iterator<std::string>()) };
}


InvertedIndex::InvertedIndex(std::istream& document_input)
{
	index.reserve(15000);

	std::string line;
	while(std::getline(document_input, line))
	{
		++docs_count_;
		size_t id = docs_count_ - 1;
		
	    for (const std::string& word : SplitIntoWords(line)) 
		{
			auto& id_to_count = index[word];
			if (!id_to_count.empty() && id_to_count.back().first == id)
			{
				++id_to_count.back().second;
			}
			else 
			{
				id_to_count.emplace_back(id, 1);
			}
		}
	}
}


const std::vector<std::pair<size_t, size_t>>& InvertedIndex::Lookup(const std::string& word) const
{
	static const std::vector<std::pair<size_t, size_t>> result;
	if (const auto it = index.find(word); it != index.end())
	{
		return it->second;
	}
	return result;
}


void UpdateDocumentBaseSingleThread(std::istream& document_input, Synchronized<InvertedIndex>& index)
{
	InvertedIndex new_index(document_input);
	std::swap(index.GetAccess().ref_to_value, new_index);
}


void AddQueriesStreamSingleThread(std::istream& query_input, 
	std::ostream& search_results_output, Synchronized<InvertedIndex>& index_)
{
    std::vector<size_t> ids_to_count;
    std::vector<int64_t> ids;

	std::string line;
	while(std::getline(query_input, line))
	{
		const auto words = SplitIntoWords(line);
		
		auto access = index_.GetAccess();
		const size_t docs_count = access.ref_to_value.GetDocsCount();
		ids_to_count.assign(docs_count, 0);
		ids.resize(docs_count);

		auto& index = access.ref_to_value;
		for (const auto& word : words) 
		{
			for (const auto& [doc_id, hit_count] : index.Lookup(word)) 
			{
				ids_to_count[doc_id] += hit_count;
			}
		}

		std::iota(ids.begin(), ids.end(), 0);
		
	    std::partial_sort(std::begin(ids), Head(ids, 5).end(), std::end(ids), 
			[&ids_to_count](int64_t lhs, int64_t rhs)
		{
			return std::pair(ids_to_count[lhs], -lhs) > std::pair(ids_to_count[rhs], -rhs);
		});

		search_results_output << line << ':';
		
	    for (size_t doc_id : Head(ids, 5)) 
		{
			const size_t hit_count = ids_to_count[doc_id];
			if (hit_count == 0) 
			{
				break;
			}

			search_results_output << " {" << "docid: " << doc_id << ", " << "hitcount: " << hit_count << '}';
		}
		
	    search_results_output << '\n';
	}
}


void SearchServer::UpdateDocumentBase(std::istream& document_input)
{
	/*
	 * futures_.push_back(std::async(UpdateDocumentBaseSingleThread, 
		ref(document_input), std::ref(index_)));
	 */

	UpdateDocumentBaseSingleThread(document_input, index_);
}


void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output)
{
	/*
	 * futures_.push_back(std::async(AddQueriesStreamSingleThread, 
		ref(query_input), ref(search_results_output), std::ref(index_)));
	 */
	AddQueriesStreamSingleThread(query_input, search_results_output, index_);
}