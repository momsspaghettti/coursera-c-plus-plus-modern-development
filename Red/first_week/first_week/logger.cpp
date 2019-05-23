#include "logger.h"


void Logger::Log(const std::string& message)
{
	if (log_file && log_line)
	{
		os << file_ << ":" << line_ << " " << message << std::endl;
	}

    else if (log_file)
    {
		os << file_ << " " << message << std::endl;
    }

    else if (log_line)
    {
		os << "Line " << line_ << " " << message << std::endl;
    }

	else
	{
		os << message << std::endl;
	}
}


void Logger::SetLine(const int& line)
{
	line_ = line;
}


void Logger::SetFile(const std::string& file)
{
	file_ = file;
}


void TestLogMacro()
{
	TestRunner tr;
	RUN_TEST(tr, TestLog);
}