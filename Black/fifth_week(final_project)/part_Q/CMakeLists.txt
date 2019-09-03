cmake_minimum_required(VERSION 3.14)
project(part_Q)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(
        PROTO_SRCS
        PROTO_HDRS
        transport_catalog.proto
)

add_executable(
        part_Q ${PROTO_SRCS} ${PROTO_HDRS} main.cpp graph.h json.h json.cpp
        navigation_database.h navigation_database.cpp router.h routes_database.h
        routes_database.cpp stops_database.h stops_database.cpp
        transport_guide_manager.h transport_guide_manager.cpp
        map_database.h map_database.cpp layers.h
        points_computer.h points_computer.cpp stops_database.h stops_builder.cpp
        svg.h svg.cpp route_map.h route_map.cpp
)

target_link_libraries(part_Q ${Protobuf_LIBRARIES})