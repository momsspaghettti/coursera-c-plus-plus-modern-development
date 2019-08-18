cmake_minimum_required(VERSION 3.14)
project(interpreter)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        interpreter mython.cpp comparators.cpp comparators.h
        object.cpp object.h object_holder.cpp
        object_holder.h object_holder_test.cpp object_test.cpp
        parse.cpp parse.h parse_test.cpp statement.cpp
        statement.h statement_test.cpp UnitTestsFramework.cpp
        UnitTestsFramework.h lexer.cpp lexer.h lexer_test.cpp
        complex_tests.h complex_tests.cpp)