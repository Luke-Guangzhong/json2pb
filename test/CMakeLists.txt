set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

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

link_libraries(libjson2pb cunit)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/log)

include_directories(${PROTO_OUTPUT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${PROTO_OUTPUT_SRC_DIR} PROTOBUF_SOURCES)

add_executable(test_public_interface src/test_public_interface.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_int32 src/test_cvt_to_int32.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_int64 src/test_cvt_to_int64.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_uint32 src/test_cvt_to_uint32.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_uint64 src/test_cvt_to_uint64.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_float src/test_cvt_to_float.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_double src/test_cvt_to_double.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_bool src/test_cvt_to_bool.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_enum src/test_cvt_to_enum.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_string src/test_cvt_to_string.c ${PROTOBUF_SOURCES})
add_executable(test_cvt_to_bytes src/test_cvt_to_bytes.c ${PROTOBUF_SOURCES})

add_test(NAME test_public_interface COMMAND test_public_interface)
add_test(NAME test_cvt_to_int32 COMMAND test_cvt_to_int32)
add_test(NAME test_cvt_to_int64 COMMAND test_cvt_to_int64)
add_test(NAME test_cvt_to_uint32 COMMAND test_cvt_to_uint32)
add_test(NAME test_cvt_to_uint64 COMMAND test_cvt_to_uint64)
add_test(NAME test_cvt_to_float COMMAND test_cvt_to_float)
add_test(NAME test_cvt_to_double COMMAND test_cvt_to_double)
add_test(NAME test_cvt_to_bool COMMAND test_cvt_to_bool)
add_test(NAME test_cvt_to_enum COMMAND test_cvt_to_enum)
add_test(NAME test_cvt_to_string COMMAND test_cvt_to_string)
add_test(NAME test_cvt_to_bytes COMMAND test_cvt_to_bytes)
