set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

link_libraries(libjson2pb)

add_executable(test_cvt_int32_t src/test_cvt_int32_t.c)

add_test(NAME foo_tests COMMAND test_cvt_int32_t)
