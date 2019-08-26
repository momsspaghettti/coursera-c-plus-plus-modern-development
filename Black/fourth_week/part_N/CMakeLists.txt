cmake_minimum_required(VERSION 3.14)
project(part_N)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        part_N main.cpp graph.h json.h json.cpp layers.h map_database.h map_database.cpp
        navigation_database.h navigation_database.cpp points_computer.h points_computer.cpp
        router.h routes_database.h routes_database.cpp stops_builder.h
        stops_builder.cpp stops_builder.h stops_builder.cpp stops_database.h stops_database.cpp
        svg.h svg.cpp test_json.h test_json.cpp test_map_database.cpp test_part_n.h
        test_part_n.cpp transport_guide_manager.h Profiler.h the_main_test.h
        transport_guide_manager.cpp UnitTestsFramework.h UnitTestsFramework.cpp
        route_map.h route_map.cpp the_main_test.cpp
)