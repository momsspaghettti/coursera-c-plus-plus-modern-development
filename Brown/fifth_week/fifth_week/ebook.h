#pragma once
#include <iostream>
#include <vector>


class BookManager
{
public:
	BookManager();

	void Read(int user, int page);

	[[nodiscard]] double Cheer(int user) const;

private:
	std::vector<int> user_to_page_;
	std::vector<int> page_to_user_count_;
	int users_count_;
};


void PerformEBookQueries(std::istream& input, std::ostream& output);


void SimpleTestForBookManager();


void TestEBook();