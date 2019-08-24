cmake_minimum_required(VERSION 3.14)
project(Optional)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        Optional main.cpp optional.h
        UnitTestsFramework.h UnitTestsFramework.cpp
        test_optional.cpp)