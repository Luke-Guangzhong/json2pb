cmake_minimum_required(VERSION 3.18)

project(cjson2pb-c)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_compile_options(-std=gnu17)
add_compile_definitions(_GNU_SOURCE)

message("c_PROJECT_SOURCE_DIR:        " ${PROJECT_SOURCE_DIR} )
message("c_CMAKE_CURRENT_BINARY_DIR:  " ${CMAKE_CURRENT_BINARY_DIR})
message("c_CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR} )
message("c_CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR} )
message("c_CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR} )

include(ExternalProject)

ExternalProject_Add(libcjson
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/cJSON
    GIT_REPOSITORY git@gitee.com:third_party_2/cJSON.git
    GIT_TAG v1.7.18
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR} -DENABLE_CJSON_TEST=On -DBUILD_SHARED_AND_STATIC_LIBS=On -DENABLE_CJSON_UTILS=On
    )
add_library(libcJSON STATIC  IMPORTED)
add_dependencies(libcJSON cJSON)

ExternalProject_Add(libtrower-base64-local
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/trower-base64
    GIT_REPOSITORY git@gitee.com:third_party_2/trower-base64.git
    GIT_TAG "v1.2.0"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR} -DBUILD_TESTING=OFF
)
add_library(libtrower-base64 STATIC IMPORTED)
add_dependencies(libtrower-base64 libtrower-base64-local)

aux_source_directory(src LIBRARY_SRC)
add_library(cjson2protobuf-c SHARED ${LIBRARY_SRC})
target_include_directories(cjson2protobuf-c PUBLIC include ${CMAKE_BINARY_DIR}/include /usr/local/include)
target_link_directories(cjson2protobuf-c PUBLIC ${CMAKE_BINARY_DIR}/lib /usr/local/lib)
target_link_libraries(cjson2protobuf-c protobuf protobuf-c cjson trower-base64)

add_subdirectory(example)