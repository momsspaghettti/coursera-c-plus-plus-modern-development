#include "UnitTestsFramework.h"
#include <vector>
#include "split_into_sentences.h"


std::ostream& operator<<(std::ostream& stream, const TestToken& token)
{
	return stream << token.data;
}


// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.

void TestSplitting() {
  
    ASSERT_EQUAL(SplitIntoSentences(std::vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})), 
		std::vector<Sentence<TestToken>>({{{"Split"}, {"into"}, {"sentences"}, {"!"}}}));

    
    ASSERT_EQUAL(SplitIntoSentences(std::vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})), 
	    std::vector<Sentence<TestToken>>({{{"Split"}, {"into"}, {"sentences"}, {"!", true}}}));

    
    ASSERT_EQUAL(SplitIntoSentences(std::vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, 
	    {"!", true}, {"Without"}, {"copies"}, {".", true}})), 
	  
	    std::vector<Sentence<TestToken>>({{{"Split"}, {"into"}, {"sentences"}, 
		    {"!", true}, {"!", true}}, {{"Without"}, {"copies"}, {".", true}},}));

    
	ASSERT_EQUAL(SplitIntoSentences(std::vector<TestToken>({ { "!", true }, { "!", true }, { "?", true },
		{ "Split" }, { "into" }, { "sentences" }, { "!", true }, { "Without" }, { "copies" } })),

		std::vector<Sentence<TestToken>>({ { { "!", true }, { "!", true }, {"?", true} }, 
			{{ "Split" }, { "into" }, { "sentences" }, { "!", true }}, {{ "Without" }, { "copies" }} }));


	ASSERT_EQUAL(SplitIntoSentences(std::vector<TestToken>({ { "!", true }, { "!", true }, { "?", true } })),

		std::vector<Sentence<TestToken>>({ { { "!", true }, { "!", true }, {"?", true} } }));


	ASSERT_EQUAL(SplitIntoSentences(std::vector<TestToken>({ {"!"}, {"!"}, {"?"} })),

		std::vector<Sentence<TestToken>>({ { {"!"}, {"!"}, {"?"} } }));


	ASSERT_EQUAL(SplitIntoSentences(std::vector<TestToken>()), std::vector<Sentence<TestToken>>());


	ASSERT_EQUAL(SplitIntoSentences(std::vector<TestToken>({ { "!", true }, { "!", true }, { "?", true },
		{ "Split" }, { "into" }, { "sentences" }, { "!", true }, { "Without" }, { "copies" }, { "!", true } })),

		std::vector<Sentence<TestToken>>({ { { "!", true }, { "!", true }, {"?", true} },
			{{ "Split" }, { "into" }, { "sentences" }, { "!", true }}, {{ "Without" }, { "copies" }, { "!", true }} }));
}


void TestSplitIntoSentences() 
{
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
}
