#pragma once
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Timer.h"


class SearchServer 
{
public:
    SearchServer() = default;
  
    explicit SearchServer(std::istream& document_input);
  
    void UpdateDocumentBase(std::istream& document_input);

    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output) const;

private:
	const static size_t N = 50000;
	std::set<std::string> words_;
    std::vector<std::unordered_map<std::string_view, int>> id_to_words_;
	std::unordered_map<std::string_view, std::vector<int>> word_to_ids_;
};