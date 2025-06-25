cmake_minimum_required(VERSION 3.18)

project(
    json2pb-convertor
    VERSION 0.0.1
    LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(POSITION_INDEPENDENT_CODE ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

if(APPLE)
    include_directories(/opt/homebrew/include /usr/local/include)
    link_directories(/opt/homebrew/lib /usr/local/lib)
    message(STATUS "Detected macOS, added /usr/local/include to include path.")
endif()

message(VERBOSE "PROJECT_SOURCE_DIR:        " ${PROJECT_SOURCE_DIR})
message(VERBOSE "CMAKE_CURRENT_BINARY_DIR:  " ${CMAKE_CURRENT_BINARY_DIR})
message(VERBOSE "CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR})
message(VERBOSE "CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR})
message(VERBOSE "CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR})

option(ENABLE_TEST "Build Test" OFF)
option(ENABLE_EXAMPLE "Build Example" OFF)

include(ExternalProject)

externalproject_add(
    external-cjson
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/cJSON
    GIT_REPOSITORY git@gitee.com:third_party_2/cJSON.git
    GIT_TAG v1.7.18
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
               -DENABLE_CJSON_TEST=ON
               -DBUILD_SHARED_AND_STATIC_LIBS=ON
               -DENABLE_CJSON_UTILS=ON
    UPDATE_COMMAND ""
    BUILD_ALWAYS OFF
    EXCLUDE_FROM_ALL TRUE)
set(LIBCJSON_FILENAME "libcjson${CMAKE_STATIC_LIBRARY_SUFFIX}")
add_library(libcJSON STATIC IMPORTED)
set_target_properties(libcJSON PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/lib/${LIBCJSON_FILENAME}")
add_dependencies(libcJSON external-cjson)
set(LIBCJSON_UTILS_FILENAME "libcjson_utils${CMAKE_STATIC_LIBRARY_SUFFIX}")
add_library(libcJSON_utils STATIC IMPORTED)
set_target_properties(libcJSON_utils PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/lib/${LIBCJSON_UTILS_FILENAME}")
add_dependencies(libcJSON_utils external-cjson)

externalproject_add(
    external-trower-base64
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/trower-base64
    GIT_REPOSITORY git@gitee.com:third_party_2/trower-base64.git
    GIT_TAG "v1.2.0"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR} -DBUILD_TESTING=OFF
    UPDATE_COMMAND ""
    BUILD_ALWAYS OFF
    EXCLUDE_FROM_ALL TRUE)
set(LIBTROWER_BASE64_FILENAME "libtrower-base64${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_library(libtrower-base64_shared SHARED IMPORTED)
set_target_properties(libtrower-base64_shared PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/lib/${LIBTROWER_BASE64_FILENAME}")
add_dependencies(libtrower-base64_shared external-trower-base64)

aux_source_directory(src LIB_SRC_LIST)

add_library(libjson2pb_shared SHARED ${LIB_SRC_LIST})
set_target_properties(
    libjson2pb_shared
    PROPERTIES POSITION_INDEPENDENT_CODE ON
               OUTPUT_NAME "json2pb"
               VERSION ${PROJECT_VERSION}
               PUBLIC_HEADER include/json2pb.h)
target_include_directories(libjson2pb_shared PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(
    libjson2pb_shared
    PUBLIC protobuf
           protobuf-c
           libcJSON
           libcJSON_utils
           libtrower-base64_shared)

add_library(libjson2pb STATIC ${LIB_SRC_LIST})
set_target_properties(
    libjson2pb
    PROPERTIES POSITION_INDEPENDENT_CODE ON
               OUTPUT_NAME "json2pb"
               VERSION ${PROJECT_VERSION}
               PUBLIC_HEADER include/json2pb.h)
target_include_directories(libjson2pb PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(
    libjson2pb
    PUBLIC protobuf
           protobuf-c
           libcJSON
           libcJSON_utils
           libtrower-base64_shared)

if(ENABLE_EXAMPLE)
    add_subdirectory(example)
endif()

if(ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
