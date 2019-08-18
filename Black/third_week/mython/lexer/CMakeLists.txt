cmake_minimum_required(VERSION 3.14)
project(mython)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        mython main.cpp lexer_test.cpp UnitTestsFramework.h
        UnitTestsFramework.cpp lexer.h lexer.cpp
)