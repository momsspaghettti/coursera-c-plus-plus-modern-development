#include "learner.h"
#include "Profiler.h"
#include "UnitTestsFramework.h"
#include <sstream>


int Learner::Learn(const std::vector<std::string>& words)
{
	const size_t old_size = dict_.size();

    for (const std::string& word : words)
    {
		dict_.insert(word);
    }

	return static_cast<int>(dict_.size() - old_size);
}


std::vector<std::string> Learner::KnownWords()
{
	std::vector<std::string> result(dict_.size());

	size_t i = 0;

	for (const std::string& word : dict_)
	{
		result[i] = word;
		++i;
	}

	return result;
}


void TestLearner()
{
	TIME_IT("TestLearner time");

	Learner learner;

    for (int i = 0; i < 3000; ++i)
    {
		learner.Learn({ 10, std::to_string(i) });

        if (i % 100 == 0)
        {
			std::ostringstream os_;

			os_ << learner.KnownWords();
        }
    }
}