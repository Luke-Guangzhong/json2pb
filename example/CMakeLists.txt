cmake_minimum_required(VERSION 3.10)

project(json2pb-c-example VERSION 0.0.1 LANGUAGES C)

message("SUB_PROJECT_SOURCE_DIR:        " ${PROJECT_SOURCE_DIR} )
message("SUB_CMAKE_CURRENT_BINARY_DIR:  " ${CMAKE_CURRENT_BINARY_DIR})
message("SUB_CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR} )
message("SUB_CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR} )
message("SUB_CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR} )

find_package(Protobuf REQUIRED)

set(PROTO_FILE ${PROJECT_SOURCE_DIR}/proto/example.proto)

set(GENERATED_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(GENERATED_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

file(MAKE_DIRECTORY ${GENERATED_SRC_DIR})
file(MAKE_DIRECTORY ${GENERATED_INCLUDE_DIR})

add_custom_command(
    OUTPUT ${GENERATED_SRC_DIR}/example.pb-c.c ${GENERATED_INCLUDE_DIR}/example.pb-c.h
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --c_out=${GENERATED_SRC_DIR} --proto_path=${PROJECT_SOURCE_DIR}/proto ${PROTO_FILE}
    COMMAND mv ${GENERATED_SRC_DIR}/example.pb-c.h ${GENERATED_INCLUDE_DIR}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating C and H files from ${PROTO_FILE}"
    VERBATIM
)

add_custom_target(generate_protobuf_files ALL
    DEPENDS ${GENERATED_SRC_DIR}/example.pb-c.c ${GENERATED_INCLUDE_DIR}/example.pb-c.h
)

add_executable(main 
    ${PROJECT_SOURCE_DIR}/src/main.c
    ${PROJECT_SOURCE_DIR}/src/example_pkg.c
    ${PROJECT_SOURCE_DIR}/src/utilities.c
    ${GENERATED_SRC_DIR}/example.pb-c.c)

add_dependencies(main generate_protobuf_files)
target_compile_options(main PUBLIC -fsanitize=address -Wunused-variable)
target_link_options(main PUBLIC -fsanitize=address)
target_include_directories(main PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    /usr/local/include)
target_link_directories(main PUBLIC 
    ${CMAKE_BINARY_DIR}/lib 
    /usr/local/lib)
target_link_libraries(main cjson2protobuf-c)
