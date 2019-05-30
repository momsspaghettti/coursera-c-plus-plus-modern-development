#include <string>
#include <string_view>
#include <stdexcept>
#include "translator.h"
#include "UnitTestsFramework.h"


void Translator::Add(std::string_view source, std::string_view target)
{
	const auto source_pos = source_to_target_.find(source);
	const auto target_pos = target_to_source_.find(target);

    if (source_pos == source_to_target_.end() && target_pos == target_to_source_.end())
    {
		storage_.push_front(std::string(source));
		storage_.push_front(std::string(target));

		source_to_target_[*std::next(storage_.begin())] = *storage_.begin();
		target_to_source_[*storage_.begin()] = *std::next(storage_.begin());
    }
	else if (target_pos == target_to_source_.end())
	{
		storage_.push_front(std::string(target));
		source_to_target_[source] = *storage_.begin();
		target_to_source_[*storage_.begin()] = source_pos->first;
	}
	else if (source_pos == source_to_target_.end())
	{
		storage_.push_front(std::string(source));
		target_to_source_[target] = *storage_.begin();
		source_to_target_[*storage_.begin()] = target_pos->first;
	}
}


std::string_view Translator::TranslateForward(std::string_view source) const
{
    try
    {
		return source_to_target_.at(source);
    }
    catch (std::out_of_range&)
    {
		return "";
    }
}


std::string_view Translator::TranslateBackward(std::string_view target) const
{
	try
	{
		return target_to_source_.at(target);
	}
	catch (std::out_of_range&)
	{
		return "";
	}
}


void SimpleTest()
{
    Translator translator;
    translator.Add(std::string("okno"), std::string("window"));
    translator.Add(std::string("stol"), std::string("table"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}


void NoSimpleTest()
{
	Translator translator;

	{
		std::string s = "okno";
		std::string t = "window";

		translator.Add(s, t);
	}

	{
		std::string s = "stol";
		std::string t = "table";

		translator.Add(s, t);
	}

	ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
	ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
	ASSERT_EQUAL(translator.TranslateBackward("stol"), "");

	{
		std::string s = "okoshko";
		std::string t = "window";

		translator.Add(s, t);
	}

	{
		std::string s = "stol";
		std::string t = "desk";

		translator.Add(s, t);
	}

	ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
	ASSERT_EQUAL(translator.TranslateForward("okoshko"), "window");
	ASSERT_EQUAL(translator.TranslateBackward("window"), "okoshko");
	ASSERT_EQUAL(translator.TranslateBackward("stol"), "");

	ASSERT_EQUAL(translator.TranslateForward("stol"), "desk");
	ASSERT_EQUAL(translator.TranslateBackward("desk"), "stol");
	ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
	ASSERT_EQUAL(translator.TranslateForward("table"), "");

	{
		std::string s1 = "roman";
		std::string t1 = "novel";
		std::string t2 = "romance";

		translator.Add(s1, t1);
		ASSERT_EQUAL(translator.TranslateForward("roman"), "novel");
		ASSERT_EQUAL(translator.TranslateBackward("novel"), "roman");

		translator.Add(s1, t2);
		ASSERT_EQUAL(translator.TranslateForward("roman"), "romance");
		ASSERT_EQUAL(translator.TranslateBackward("novel"), "roman");
		ASSERT_EQUAL(translator.TranslateBackward("romance"), "roman");
	}

	ASSERT_EQUAL(translator.TranslateBackward("novel"), "roman");
	ASSERT_EQUAL(translator.TranslateForward("roman"), "romance");
	ASSERT_EQUAL(translator.TranslateBackward("romance"), "roman");
}


void TestTranslator() 
{
    TestRunner tr;
    RUN_TEST(tr, SimpleTest);
	RUN_TEST(tr, NoSimpleTest);
}
