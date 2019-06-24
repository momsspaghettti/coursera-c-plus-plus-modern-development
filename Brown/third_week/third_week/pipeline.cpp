#include "UnitTestsFramework.h"
#include <sstream>
#include "pipeline.h"


void TestSanity() 
{
    std::string input = (
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"ralph@example.com\n"
		"Are you sure you pressed the right button?\n"

		"ralph@example.com\n"
		"erich@example.com\n"
		"I do not make mistakes of that kind\n"
		);
    
    std::istringstream inStream(input);
    std::ostringstream outStream, outStream2;

	PipelineBuilder builder(inStream);
	
    builder.FilterBy([](const Email& email) 
	{
		return email.from == "erich@example.com";
	});

	builder.CopyTo("richard@example.com");
	builder.Send(outStream);
	
    builder.FilterBy([](const Email& email)
	{
		return email.to == "richard@example.com" &&
			email.from == "erich@example.com";
	});
	builder.Send(outStream2);
	
    auto pipeline = builder.Build();

	pipeline->Run();

	std::string expectedOutput = (
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"ralph@example.com\n"
		"Are you sure you pressed the right button?\n"

		"erich@example.com\n"
		"richard@example.com\n"
		"Are you sure you pressed the right button?\n"
		);

	ASSERT_EQUAL(expectedOutput, outStream.str());

	std::string expectedOutput2(
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"richard@example.com\n"
		"Are you sure you pressed the right button?\n"
	);

	ASSERT_EQUAL(expectedOutput2, outStream2.str());
}


void TestPipeLine() 
{
	TestRunner tr;
	RUN_TEST(tr, TestSanity);
}