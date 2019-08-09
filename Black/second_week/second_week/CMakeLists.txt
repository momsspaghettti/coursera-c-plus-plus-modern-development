cmake_minimum_required(VERSION 3.14)
project(second_week)

set(CMAKE_CXX_STANDARD 17)

add_executable(second_week main.cpp UnitTestsFramework.h UnitTestsFramework.cpp json_printer.h test_json_printer.cpp json_printer.cpp save_load.h test_save_load.cpp)