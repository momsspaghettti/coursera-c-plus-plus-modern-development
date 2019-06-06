#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class SearchServer 
{
public:
    SearchServer() = default;
  
    explicit SearchServer(std::istream& document_input);
  
    void UpdateDocumentBase(std::istream& document_input);

    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output) const;

private:
	const static size_t N = 50000;
    std::vector<std::unordered_map<std::string, int>> id_to_words_;
	std::unordered_map<std::string, std::vector<int>> word_to_ids_;
	size_t docs_count = 0;
};