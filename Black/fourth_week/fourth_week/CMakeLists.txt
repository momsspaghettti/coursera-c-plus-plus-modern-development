cmake_minimum_required(VERSION 3.14)
project(fourth_week)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        fourth_week main.cpp UnitTestsFramework.h UnitTestsFramework.cpp
        equals_to_one_of.h
        apply_to_many.h)