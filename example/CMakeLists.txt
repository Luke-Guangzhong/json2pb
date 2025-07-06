cmake_minimum_required(VERSION 3.18)

message(VERBOSE "PROJECT_SOURCE_DIR:        " ${PROJECT_SOURCE_DIR})
message(VERBOSE "CMAKE_CURRENT_BINARY_DIR:  " ${CMAKE_CURRENT_BINARY_DIR})
message(VERBOSE "CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR})
message(VERBOSE "CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR})
message(VERBOSE "CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR})

include(ExternalProject)

externalproject_add(
    external-cimplog
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/cimplog
    GIT_REPOSITORY git@gitee.com:third_party_2/cimplog.git
    GIT_TAG "49b84f1462c2ce9e8249f94c9492b5f3e2269b02"
    CMAKE_ARGS
               -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
               -DBUILD_TESTING=OFF
               -DLEVEL_DEFAULT=2
    UPDATE_COMMAND ""
    BUILD_ALWAYS OFF
    EXCLUDE_FROM_ALL TRUE)
add_library(libcimplog STATIC IMPORTED)
set_target_properties(libcimplog PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/lib/libcimplog${CMAKE_STATIC_LIBRARY_SUFFIX}")
add_dependencies(libcimplog external-cimplog)

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
    ERROR_VARIABLE error)

execute_process(
    COMMAND protoc ${MSG_PROTO_FILES} --python_out=${PROJECT_SOURCE_DIR}/python -I ${PROTO_SRC_DIR}
    WORKING_DIRECTORY ${PROTO_SRC_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)

file(GLOB PROTO_H_FILES ${PROTO_OUTPUT_SRC_DIR}/*.pb-c.h)

execute_process(
    COMMAND mv ${PROTO_H_FILES} ${PROTO_OUTPUT_INCLUDE_DIR}
    WORKING_DIRECTORY ${PROTO_OUTPUT_SRC_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)

aux_source_directory(src EXE_SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_BINARY_DIR}/src BUILD_SRC_LIST)

add_executable(TEST_EXE ${EXE_SRC_LIST} ${BUILD_SRC_LIST})
set_target_properties(
    TEST_EXE
    PROPERTIES POSITION_INDEPENDENT_CODE ON
               OUTPUT_NAME "json2pb"
               VERSION ${PROJECT_VERSION})
target_include_directories(TEST_EXE PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_directories(
    TEST_EXE
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/lib
    ${LIBRARY_OUTPUT_PATH})
target_link_libraries(TEST_EXE PRIVATE libjson2pb libcimplog)
