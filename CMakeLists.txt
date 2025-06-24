cmake_minimum_required(VERSION 3.18)

project(
    json2pb-convertor
    VERSION 0.0.1
    LANGUAGES C)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_compile_options(-std=gnu17)
add_compile_definitions(_GNU_SOURCE)

if(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    message(STATUS "Detected macOS, added /usr/local/include to include path.")
endif()

message(VERBOSE "PROJECT_SOURCE_DIR:        " ${PROJECT_SOURCE_DIR})
message(VERBOSE "CMAKE_CURRENT_BINARY_DIR:  " ${CMAKE_CURRENT_BINARY_DIR})
message(VERBOSE "CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR})
message(VERBOSE "CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR})
message(VERBOSE "CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR})

option(ENABLE_TEST "Build Test" OFF)
option(DEBUG_BUT_NOT_ASSERT "Enable debug but not assert" OFF)

include(ExternalProject)

externalproject_add(
    external-cjson
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/cJSON
    GIT_REPOSITORY git@gitee.com:third_party_2/cJSON.git
    GIT_TAG v1.7.18
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
               -DENABLE_CJSON_TEST=ON
               -DBUILD_SHARED_AND_STATIC_LIBS=ON
               -DENABLE_CJSON_UTILS=ON)
add_library(libcJSON STATIC IMPORTED)
set_target_properties(libcJSON PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libcjson.a)
add_dependencies(libcJSON external-cjson)
add_library(libcJSON_utils STATIC IMPORTED)
set_target_properties(libcJSON_utils PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libcjson_utils.a)
add_dependencies(libcJSON_utils external-cjson)

externalproject_add(
    external-trower-base64
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/trower-base64
    GIT_REPOSITORY git@gitee.com:third_party_2/trower-base64.git
    GIT_TAG "v1.2.0"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR} -DBUILD_TESTING=OFF)
add_library(libtrower-base64 SHARED IMPORTED)
set_target_properties(libtrower-base64 PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libtrower-base64.so)
add_dependencies(libtrower-base64 libtrower-base64-local)

aux_source_directory(src LIB_SRC_LIST)

add_library(SHARED_LIBRARY SHARED ${LIB_SRC_LIST})
set_target_properties(
    SHARED_LIBRARY
    PROPERTIES POSITION_INDEPENDENT_CODE ON
               OUTPUT_NAME "json2pb"
               VERSION ${PROJECT_VERSION}
               PUBLIC_HEADER include/json2pb.h)
target_include_directories(SHARED_LIBRARY PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_directories(SHARED_LIBRARY PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib)
target_link_libraries(
    SHARED_LIBRARY
    protobuf
    protobuf-c
    libcJSON
    libtrower-base64
    libcJSON_utils)
if(DEBUG_BUT_NOT_ASSERT)
    target_compile_definitions(SHARED_LIBRARY PUBLIC REMOVE_ASSERT)
endif()

if(ENABLE_TEST)
    add_subdirectory(test)
endif()
