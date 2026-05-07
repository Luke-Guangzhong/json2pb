cmake_minimum_required(VERSION 3.18)

project(
    json2pb-convertor
    VERSION 0.0.2
    LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(POSITION_INDEPENDENT_CODE ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_BUILD_RPATH ${CMAKE_BINARY_DIR}/lib)

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

include(ExternalProject)

externalproject_add(
    external-cjson
    PREFIX ${CMAKE_BINARY_DIR}/third-party/cJSON
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
    GIT_TAG "master"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
               -DENABLE_CJSON_TEST=OFF
               -DBUILD_SHARED_AND_STATIC_LIBS=ON
               -DENABLE_CJSON_UTILS=ON
    UPDATE_COMMAND ""
    BUILD_ALWAYS OFF
    EXCLUDE_FROM_ALL TRUE
    BUILD_BYPRODUCT "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}cjson${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}cjson_utils${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_library(libcJSON_shared SHARED IMPORTED)
set_target_properties(libcJSON_shared PROPERTIES IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}cjson${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_dependencies(libcJSON_shared external-cjson)
add_library(libcJSON_utils_shared SHARED IMPORTED)
set_target_properties(libcJSON_utils_shared PROPERTIES IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}cjson_utils${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_dependencies(libcJSON_utils_shared external-cjson)

externalproject_add(
    external-trower-base64
    PREFIX ${CMAKE_BINARY_DIR}/third-party/trower-base64
    GIT_REPOSITORY https://github.com/xmidt-org/trower-base64.git
    GIT_TAG "v1.2.0"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR} -DBUILD_TESTING=OFF
    UPDATE_COMMAND ""
    BUILD_ALWAYS OFF
    EXCLUDE_FROM_ALL TRUE
    BUILD_BYPRODUCT "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}trower-base64${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_library(libtrower-base64_shared SHARED IMPORTED)
set_target_properties(libtrower-base64_shared PROPERTIES IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}trower-base64${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_dependencies(libtrower-base64_shared external-trower-base64)

add_compile_definitions(_GNU_SOURCE)

add_library(libjson2pb_shared SHARED)
target_sources(
    libjson2pb_shared
    PRIVATE src/json2pb.c
            src/cvt.c
            src/default.c
            src/easy_interface.c
            src/utils.c
    PUBLIC include/json2pb.h)
set_target_properties(
    libjson2pb_shared
    PROPERTIES POSITION_INDEPENDENT_CODE ON
               OUTPUT_NAME "json2pb"
               VERSION ${PROJECT_VERSION}
               PUBLIC_HEADER include/json2pb.h
               C_VISIBILITY_PRESET hidden
               VISIBILITY_INLINES_HIDDEN YES)
target_include_directories(libjson2pb_shared PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(
    libjson2pb_shared
    PUBLIC protobuf
           protobuf-c
           libcJSON_shared
           libcJSON_utils_shared
           libtrower-base64_shared)

if(ENABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
