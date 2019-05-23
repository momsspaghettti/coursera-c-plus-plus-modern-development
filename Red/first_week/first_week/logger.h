#pragma once
#include "UnitTestsFramework.h"
#include <sstream>
#include <string>
#include "sstream"


class Logger
{
public:
	explicit Logger(std::ostream& output_stream) : os(output_stream) {}

	void SetLogLine(bool value) { log_line = value; }
	void SetLogFile(bool value) { log_file = value; }

	void SetFile(const std::string& file);
	void SetLine(const int& line);

	void Log(const std::string& message);

private:
	std::ostream& os;
	bool log_line = false;
	bool log_file = false;

	std::string file_;
	int line_;
};


#define LOG(logger, message)                                                         \
{                                                                                    \
    logger.SetFile(__FILE__);                                                        \
    logger.SetLine(__LINE__);                                                        \
    logger.Log(message);                                                             \
}


inline void TestLog() {
	/* Для написания юнит-тестов в этой задаче нам нужно фиксировать конкретные
	 * номера строк в ожидаемом значении (см. переменную expected ниже). Если
	 * мы добавляем какой-то код выше функции TestLog, то эти номера строк меняются,
	 * и наш тест начинает падать. Это неудобно.
	 *
	 * Чтобы этого избежать, мы используем специальный макрос #line
	 * (http://en.cppreference.com/w/cpp/preprocessor/line), который позволяет
	 * переопределить номер строки, а также имя файла. Благодаря ему, номера
	 * строк внутри функции TestLog будут фиксированы независимо от того, какой
	 * код мы добавляем перед ней*/

#line 1 "logger.cpp"

	std::ostringstream logs;
	Logger l(logs);
	LOG(l, "hello");

	l.SetLogFile(true);
	LOG(l, "hello");

	l.SetLogLine(true);
	LOG(l, "hello");

	l.SetLogFile(false);
	LOG(l, "hello");

	std::string expected = "hello\n";
	expected += "logger.cpp hello\n";
	expected += "logger.cpp:10 hello\n";
	expected += "Line 13 hello\n";
	ASSERT_EQUAL(logs.str(), expected);
}


void TestLogMacro();