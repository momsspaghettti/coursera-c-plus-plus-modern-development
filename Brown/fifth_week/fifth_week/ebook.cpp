#include "ebook.h"
#include "UnitTestsFramework.h"
#include <sstream>
#include <string>


BookManager::BookManager()
{
	user_to_page_ = std::vector<int>(100'001);
	std::fill(user_to_page_.begin(), user_to_page_.end(), -1);

	page_to_user_count_ = std::vector<int>(1001);
	users_count_ = 0;
}


void BookManager::Read(int user, int page)
{
	int& current_page = user_to_page_[user];

    if (current_page == -1)
    {
		++page_to_user_count_[page];
		++users_count_;
    }
    else
    {
		--page_to_user_count_[current_page];
		++page_to_user_count_[page];
    }

	current_page = page;
}


double BookManager::Cheer(int user) const
{
	const int page = user_to_page_.at(user);

	if (page == -1)
		return 0;

	if (users_count_ == 1)
		return 1;

	double answer = 0;
    for (int i = 0; i < page; ++i)
    {
		answer += page_to_user_count_.at(i);
    }

	return answer / (users_count_ - 1);
}


void PerformEBookQueries(std::istream& input, std::ostream& output)
{
	input.tie(nullptr);
	std::ios_base::sync_with_stdio(false);

	BookManager book_manager;

	int n;
	input >> n;

	std::string query;
	int user;

	for (int i = 0; i < n; ++i)
	{
		input >> query;

        if (query == "READ")
        {
			int page;
			input >> user >> page;

			book_manager.Read(user, page);
        }

        if (query == "CHEER")
        {
			input >> user;

			output << book_manager.Cheer(user) << '\n';
        }
	}
}


void SimpleTestForBookManager()
{
	BookManager book_manager;

	ASSERT_EQUAL(book_manager.Cheer(5), 0);
	
    book_manager.Read(1, 10);
	ASSERT_EQUAL(book_manager.Cheer(1), 1);
	
    book_manager.Read(2, 5);
	book_manager.Read(3, 7);
	ASSERT_EQUAL(book_manager.Cheer(2), 0);
	ASSERT_EQUAL(book_manager.Cheer(3), 0.5);

	book_manager.Read(3, 10);
	ASSERT_EQUAL(book_manager.Cheer(3), 0.5);

	book_manager.Read(3, 11);
	ASSERT_EQUAL(book_manager.Cheer(3), 1);
	ASSERT_EQUAL(book_manager.Cheer(1), 0.5);

	book_manager.Read(10, 11);
	ASSERT_EQUAL(book_manager.Cheer(10), 2 / static_cast<double>(3));

	book_manager.Read(5, 11);
	ASSERT_EQUAL(book_manager.Cheer(5), 0.5);
}


void TestEBookQueries()
{
	std::string queries = 
		"12\n"
        "CHEER 5\n"
        "READ 1 10\n"
        "CHEER 1\n"
        "READ 2 5\n"
        "READ 3 7\n"
        "CHEER 2\n"
        "CHEER 3\n"
        "READ 3 10\n"
        "CHEER 3\n"
        "READ 3 11\n"
        "CHEER 3\n"
        "CHEER 1\n";
	
    std::istringstream input(queries);

	std::string expected =
		"0\n"
		"1\n"
		"0\n"
		"0.5\n"
		"0.5\n"
		"1\n"
		"0.5\n";

	std::ostringstream output;

	PerformEBookQueries(input, output);
	ASSERT_EQUAL(output.str(), expected);
}


void TestEBook()
{
	TestRunner tr;
	RUN_TEST(tr, SimpleTestForBookManager);
	RUN_TEST(tr, TestEBookQueries);
}