cmake_minimum_required(VERSION 3.14)
project(part_O)

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
        part_O ${PROTO_SRCS} ${PROTO_HDRS} main.cpp json.h json.cpp
        routes_database.h routes_database.cpp stops_database.h
        stops_database.cpp transport_guide_manager.h transport_guide_manager.cpp
)

target_link_libraries(part_O ${Protobuf_LIBRARIES})