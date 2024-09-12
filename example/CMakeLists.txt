cmake_minimum_required(VERSION 3.10)

project(json2pb-c-example)

message("SUB_PROJECT_SOURCE_DIR:        " ${PROJECT_SOURCE_DIR} )
message("SUB_CMAKE_CURRENT_BINARY_DIR:  " ${CMAKE_CURRENT_BINARY_DIR})
message("SUB_CMAKE_BINARY_DIR:          " ${CMAKE_BINARY_DIR} )
message("SUB_CMAKE_CURRENT_SOURCE_DIR:  " ${CMAKE_CURRENT_SOURCE_DIR} )
message("SUB_CMAKE_SOURCE_DIR:          " ${CMAKE_SOURCE_DIR} )

# 设置 Protobuf 的编译器
find_package(Protobuf REQUIRED)

# 指定 Protobuf 文件
set(PROTO_FILE ${PROJECT_SOURCE_DIR}/proto/example.proto)

# 设置输出目录
set(GENERATED_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)

# 创建生成文件目录
file(MAKE_DIRECTORY ${GENERATED_SRC_DIR})
file(MAKE_DIRECTORY ${GENERATED_INCLUDE_DIR})

# 生成 .c 和 .h 文件的命令
add_custom_command(
    OUTPUT ${GENERATED_SRC_DIR}/example.pb-c.c ${GENERATED_INCLUDE_DIR}/example.pb-c.h
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --c_out=${GENERATED_SRC_DIR} --proto_path=${PROJECT_SOURCE_DIR}/proto ${PROTO_FILE}
    COMMAND mv ${GENERATED_SRC_DIR}/example.pb-c.h ${GENERATED_INCLUDE_DIR}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating C and H files from ${PROTO_FILE}"
    VERBATIM
)

# 添加一个目标，确保生成 .c 和 .h 文件
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
