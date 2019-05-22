#include "pch.h"
#include "phone_number_tests.h"
#include "UnitTestsFramework.h"
#include "phone_number.h"


void TestValidPhoneNumberConstructor()
{
	PhoneNumber phone_number("+7-495-111-22-33");

	AssertEqual(phone_number.GetCountryCode(), "7", "country code");
	AssertEqual(phone_number.GetCityCode(), "495", "city code");
	AssertEqual(phone_number.GetLocalNumber(), "111-22-33", "local number");
	AssertEqual(phone_number.GetInternationalNumber(), "+7-495-111-22-33", "international number");

	phone_number = PhoneNumber("+7-495-1112233");
	AssertEqual(phone_number.GetCountryCode(), "7", "country code");
	AssertEqual(phone_number.GetCityCode(), "495", "city code");
	AssertEqual(phone_number.GetLocalNumber(), "1112233", "local number");
	AssertEqual(phone_number.GetInternationalNumber(), "+7-495-1112233", "international number");

	phone_number = PhoneNumber("+323-22-460002");
	AssertEqual(phone_number.GetCountryCode(), "323", "country code");
	AssertEqual(phone_number.GetCityCode(), "22", "city code");
	AssertEqual(phone_number.GetLocalNumber(), "460002", "local number");
	AssertEqual(phone_number.GetInternationalNumber(), "+323-22-460002", "international number");

	phone_number = PhoneNumber("+1-2-coursera-cpp");
	AssertEqual(phone_number.GetCountryCode(), "1", "country code");
	AssertEqual(phone_number.GetCityCode(), "2", "city code");
	AssertEqual(phone_number.GetLocalNumber(), "coursera-cpp", "local number");
	AssertEqual(phone_number.GetInternationalNumber(), "+1-2-coursera-cpp", "international number");

	phone_number = PhoneNumber("+1-2-3");
	AssertEqual(phone_number.GetCountryCode(), "1", "country code");
	AssertEqual(phone_number.GetCityCode(), "2", "city code");
	AssertEqual(phone_number.GetLocalNumber(), "3", "local number");
	AssertEqual(phone_number.GetInternationalNumber(), "+1-2-3", "international number");
}


void TestInvalidPhoneNumberConstructor()
{
	try
	{
		PhoneNumber phone_number("1-2-333");
	}
	catch (std::invalid_argument& ex)
	{
		std::string message = ex.what();
		AssertEqual(message, "number without '+'!", "plus check");
	}

	try
	{
		PhoneNumber phone_number("+");
	}
	catch (std::invalid_argument& ex)
	{
		std::string message = ex.what();
		AssertEqual(message, "no country code!", "country code check");
	}

	try
	{
		PhoneNumber phone_number("+123-");
	}
	catch (std::invalid_argument& ex)
	{
		std::string message = ex.what();
		AssertEqual(message, "no city code!", "city code check");
	}

	try
	{
		PhoneNumber phone_number("+7-1233");
	}
	catch (std::invalid_argument& ex)
	{
		std::string message = ex.what();
		AssertEqual(message, "empty local number!", "local number check");
	}

	try
	{
		PhoneNumber phone_number("");
	}
	catch (std::invalid_argument& ex)
	{
		std::string message = ex.what();
		AssertEqual(message, "empty number!", "empty number check");
	}
}


void TestAllPhoneNumberTests()
{
	TestRunner test_runner;

	test_runner.RunTest(TestValidPhoneNumberConstructor, "TestValidPhoneNumberConstructor");
	test_runner.RunTest(TestInvalidPhoneNumberConstructor, "TestInvalidPhoneNumberConstructor");
}