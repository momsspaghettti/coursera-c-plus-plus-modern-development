cmake_minimum_required(VERSION 3.14)
project(part_L)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        part_L main.cpp graph.h json.h json.cpp map_database.cpp map_database.h
        navigation_database.h navigation_database.cpp Profiler.h router.h
        routes_database.h routes_database.cpp stops_database.h stops_database.cpp
        svg.h svg.cpp test_json.h test_json.cpp test_map_database.cpp test_part_l.h
        test_part_l.cpp the_main_test.h the_main_test.cpp transport_guide_manager.h
        transport_guide_manager.cpp UnitTestsFramework.h UnitTestsFramework.cpp
        layers.h points_computer.cpp points_computer.h stops_builder.cpp stops_builder.h)