cmake_minimum_required(VERSION 3.18)

project(json2pb-convertor VERSION 0.0.1 LANGUAGES C)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_compile_options(-std=gnu17)
add_compile_definitions(_GNU_SOURCE)

if(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    message(STATUS "Detected macOS, added /usr/local/include to include path.")
endif()

message("c_PROJECT_SOURCE_DIR:        " ${PROJECT_SOURCE_DIR} )
message("c_CMAKE_CURRENT_BINARY_DIR:  " ${CMAKE_CURRENT_BINARY_DIR})
message("c_CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR} )
message("c_CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR} )
message("c_CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR} )

option(ENABLE_EXAMPLES "Build examples" OFF)

include(ExternalProject)

ExternalProject_Add(libcjson
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/cJSON
    GIT_REPOSITORY git@gitee.com:third_party_2/cJSON.git
    GIT_TAG v1.7.18
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR} -DENABLE_CJSON_TEST=On -DBUILD_SHARED_AND_STATIC_LIBS=On -DENABLE_CJSON_UTILS=On
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

ExternalProject_Add(cimplog
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/cimplog
    GIT_REPOSITORY git@gitee.com:third_party_2/cimplog.git
    GIT_TAG "1.0.1"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR} -DBUILD_TESTING=OFF -DLEVEL_DEFAULT=2
)
add_library(libcimplog STATIC IMPORTED)
set_target_properties(libcimplog PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/lib/libcimplog.a"
)
add_dependencies(libcimplog cimplog)

set(PROTOC_C_EXECUTABLE protoc-c)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protobuf)
set(PROTO_OUTPUT_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(PROTO_OUTPUT_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)

file(MAKE_DIRECTORY ${PROTO_OUTPUT_SRC_DIR})
file(MAKE_DIRECTORY ${PROTO_OUTPUT_INCLUDE_DIR})

file(GLOB MSG_PROTO_FILES ${PROTO_SRC_DIR}/*.proto)

execute_process(
    COMMAND protoc-c ${MSG_PROTO_FILES} --c_out=${PROTO_OUTPUT_SRC_DIR} -I ${PROTO_SRC_DIR}
    WORKING_DIRECTORY ${PROTO_SRC_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

execute_process(
    COMMAND protoc ${MSG_PROTO_FILES} --python_out=${PROJECT_SOURCE_DIR}/python -I ${PROTO_SRC_DIR}
    WORKING_DIRECTORY ${PROTO_SRC_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

file(GLOB PROTO_H_FILES ${PROTO_OUTPUT_SRC_DIR}/*.pb-c.h)

execute_process(
    COMMAND mv ${PROTO_H_FILES} ${PROTO_OUTPUT_INCLUDE_DIR}
    WORKING_DIRECTORY ${PROTO_OUTPUT_SRC_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error
)

option(DEBUG_BUT_NOT_ASSERT "Enable debug but not assert" OFF)

aux_source_directory(src/libjson2pb LIB_SRC_LIST)

add_library(SHARED_LIBRARY SHARED ${LIB_SRC_LIST})
set_target_properties(SHARED_LIBRARY PROPERTIES POSITION_INDEPENDENT_CODE ON OUTPUT_NAME "json2pb" VERSION ${PROJECT_VERSION})
target_include_directories(SHARED_LIBRARY PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_directories(SHARED_LIBRARY PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib )
target_link_libraries(SHARED_LIBRARY protobuf protobuf-c cjson trower-base64 cjson_utils)
if(DEBUG_BUT_NOT_ASSERT)
    target_compile_definitions(SHARED_LIBRARY PUBLIC REMOVE_ASSERT)
endif()

aux_source_directory(src EXE_SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_BINARY_DIR}/src BUILD_SRC_LIST)

add_executable(TEST_EXE ${EXE_SRC_LIST} ${BUILD_SRC_LIST})
set_target_properties(TEST_EXE PROPERTIES POSITION_INDEPENDENT_CODE ON OUTPUT_NAME "json2pb" VERSION ${PROJECT_VERSION})
target_include_directories(TEST_EXE PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_directories(TEST_EXE PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib ${LIBRARY_OUTPUT_PATH})
target_link_libraries(TEST_EXE SHARED_LIBRARY libcimplog.a)
if(DEBUG_BUT_NOT_ASSERT)
    target_compile_definitions(SHARED_LIBRARY PUBLIC REMOVE_ASSERT)
endif()