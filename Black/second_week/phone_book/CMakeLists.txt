cmake_minimum_required(VERSION 3.14)
project(phone_book)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS contact.proto)

add_executable(
        phone_book main.cpp phone_book.h iterator_range.h
        UnitTestsFramework.cpp UnitTestsFramework.h
        phone_book.cpp ${PROTO_SRCS} ${PROTO_HDRS}
        test_phone_book.cpp
)

target_link_libraries(phone_book ${Protobuf_LIBRARIES})