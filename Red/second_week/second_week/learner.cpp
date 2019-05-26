#include "learner.h"
#include "Profiler.h"
#include "UnitTestsFramework.h"
#include <sstream>


int Learner::Learn(const std::vector<std::string>& words)
{
	const size_t old_size = dict_.size();

	dict_.insert(words.begin(), words.end());

	return static_cast<int>(dict_.size() - old_size);
}


std::vector<std::string> Learner::KnownWords()
{
    return { dict_.begin(), dict_.end() };
}


void TestLearner()
{
	TIME_IT("TestLearner time");

	Learner learner;

    for (int i = 0; i < 4000; ++i)
    {
		learner.Learn({ 10, std::to_string(i) });

        if (i % 100 == 0)
        {
			std::ostringstream os_;

			os_ << learner.KnownWords();
        }
    }
}