cmake_minimum_required(VERSION 3.14)
project(third_week)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        third_week main.cpp nucleotide.h nucleotide.cpp
        nucleotide_test.cpp UnitTestsFramework.cpp
        UnitTestsFramework.h
)