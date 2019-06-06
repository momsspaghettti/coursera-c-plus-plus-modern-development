#pragma once
#include <istream>
#include <vector>
#include <unordered_map>
#include <string>
#include <future>
#include "synchronized.h"


class InvertedIndex
{
public:
	InvertedIndex() = default;
	
    explicit InvertedIndex(std::istream& document_input);

    const std::vector<std::pair<size_t, size_t>>& Lookup(const std::string& word) const;

    size_t GetDocsCount() const
    {
		return docs_count_;
    }

private:
	size_t docs_count_ = 0;
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> index;
};


class SearchServer
{
public:
	SearchServer() = default;

	explicit SearchServer(std::istream& document_input)
		: index_(InvertedIndex(document_input)){}

	void UpdateDocumentBase(std::istream& document_input);
	
    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

private:
	Synchronized<InvertedIndex> index_;
    std::vector<std::future<void>> futures_;
};