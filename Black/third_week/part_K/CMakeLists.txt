cmake_minimum_required(VERSION 3.14)
project(part_J)

set(CMAKE_CXX_STANDARD 17)

add_executable(part_K graph.h json.cpp json.h test_json.cpp
        test_json.h main.cpp navigation_database.cpp
        navigation_database.h Profiler.h router.h routes_database.cpp
        routes_database.h stops_database.cpp stops_database.h
        test_part_k.cpp test_part_k.h the_main_test.cpp the_main_test.h
        transport_guide_manager.cpp transport_guide_manager.h
        UnitTestsFramework.cpp UnitTestsFramework.h svg.cpp svg.h map_database.h map_database.cpp
        test_map_database.cpp)