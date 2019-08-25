cmake_minimum_required(VERSION 3.14)
project(Vector)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        Vector main.cpp vector.h test_vector.cpp
        UnitTestsFramework.h UnitTestsFramework.cpp
)