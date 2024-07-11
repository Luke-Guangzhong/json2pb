cmake_minimum_required(VERSION 3.18)

project(json2pb-c)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

message("c_CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR} )
message("c_CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR} )
message("c_CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR} )

add_executable(main test/main.c)

add_library(json2protobuf-c SHARED src/protobuf-c-utilities.c)

target_include_directories(json2protobuf-c PUBLIC include)