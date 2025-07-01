/**
 * @file test_cvt_to_int32_t.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <unistd.h>

#include "json2pb.h"
#include "test.pb-c.h"
#include "utils.h"

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

void test_cvt_json_number_to_single_int32(void);
void test_cvt_json_decimal_string_to_single_int32(void);
void test_cvt_json_hex_string_to_single_int32(void);
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void test_cvt_json_binary_string_to_single_int32(void);
#endif
void test_cvt_json_octal_string_to_single_int32(void);
void test_cvt_json_number_to_oneof_int32(void);

void test_cvt_json_number_overflow_int32(void);
void test_cvt_json_number_underflow_int32(void);
void test_cvt_json_str_overflow_int32(void);
void test_cvt_json_str_underflow_int32(void);
void test_cvt_json_number_max_int32(void);
void test_cvt_json_number_min_int32(void);
void test_cvt_json_str_max_int32(void);
void test_cvt_json_str_min_int32(void);

void test_cvt_json_array_to_repeated_int32(void);
void test_cvt_json_array_to_repeated_int32_partly_failed(void);
void test_cvt_json_array_to_repeated_int32_all_failed(void);
void test_cvt_json_array_to_repeated_int32_empty(void);

void test_cvt_json_number_to_sint32(void);
void test_cvt_json_array_to_repeated_sint32(void);
void test_cvt_json_number_to_sfixed32(void);
void test_cvt_json_array_to_repeated_sfixed32(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char int32_field_name[]             = "f_int32";
const char sint32_field_name[]            = "f_sint32";
const char sfixed32_field_name[]          = "f_sfixed32";
const char repeated_int32_field_name[]    = "f_repeated_int32";
const char repeated_sint32_field_name[]   = "f_repeated_sint32";
const char repeated_sfixed32_field_name[] = "f_repeated_sfixed32";

CU_TestInfo test_int32_conversion[] = {
    {"Convert JSON number to Protobuf int32 field",             test_cvt_json_number_to_single_int32        },
    {"Convert JSON decimal string to Protobuf int32 field",     test_cvt_json_decimal_string_to_single_int32},
    {"Convert JSON hexadecimal string to Protobuf int32 field", test_cvt_json_hex_string_to_single_int32    },
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    {"Convert JSON binary string to Protobuf int32 field",      test_cvt_json_binary_string_to_single_int32 },
#endif
    {"Convert JSON octal string to Protobuf int32 field",       test_cvt_json_octal_string_to_single_int32  },
    {"Convert JSON number to Protobuf oneof int32 field",       test_cvt_json_number_to_oneof_int32         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_int32_overflow[] = {
    {"Reject conversion of JSON number to Protobuf int32 on overflow",  test_cvt_json_number_overflow_int32 },
    {"Reject conversion of JSON number to Protobuf int32 on underflow", test_cvt_json_number_underflow_int32},
    {"Reject conversion of JSON string to Protobuf int32 on overflow",  test_cvt_json_str_overflow_int32    },
    {"Reject conversion of JSON string to Protobuf int32 on underflow", test_cvt_json_str_underflow_int32   },
    {"Accept conversion of JSON number to Protobuf int32 at INT32_MAX", test_cvt_json_number_max_int32      },
    {"Accept conversion of JSON number to Protobuf int32 at INT32_MIN", test_cvt_json_number_min_int32      },
    {"Accept conversion of JSON string to Protobuf int32 at INT32_MAX", test_cvt_json_str_max_int32         },
    {"Accept conversion of JSON string to Protobuf int32 at INT32_MIN", test_cvt_json_str_min_int32         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_int32_conversion[] = {
    {"Convert JSON array to repeated Protobuf int32 field",              test_cvt_json_array_to_repeated_int32              },
    {"Partial failure converting JSON array to repeated Protobuf int32", test_cvt_json_array_to_repeated_int32_partly_failed},
    {"All failure converting JSON array to repeated Protobuf int32",     test_cvt_json_array_to_repeated_int32_all_failed   },
    {"Convert Empty JSON array to repeated Protobuf int32",              test_cvt_json_array_to_repeated_int32_empty        },
    CU_TEST_INFO_NULL
};

CU_TestInfo test_xs32_conversion[] = {
    {"Convert JSON number to Protobuf sint32 field",           test_cvt_json_number_to_sint32          },
    {"Convert JSON array to Protobuf repeated sint32 field",   test_cvt_json_array_to_repeated_sint32  },
    {"Convert JSON number to Protobuf sfixed32 field",         test_cvt_json_number_to_sfixed32        },
    {"Convert JSON array to Protobuf repeated sfixed32 field", test_cvt_json_array_to_repeated_sfixed32},
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf int32 field",                  init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_int32_conversion},
    {"Convert JSON to Protobuf int32 with overflow handling", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_int32_overflow                                                                                                                                                             },
    {"Convert JSON array to repeated Protobuf int32 field",   init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_repeated_int32_conversion                                                                                                                                                  },
    {"Convert JSON to Protobuf sint32/sfixed32 field",        init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_xs32_conversion },
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_number_to_single_int32(void)
{
    int32_t value = 1234567890;

    cJSON_AddNumberToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, value);
}

void
test_cvt_json_decimal_string_to_single_int32(void)
{
    const char* value = "1234567890";

    cJSON_AddStringToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, 1234567890);
}

void
test_cvt_json_hex_string_to_single_int32(void)
{
    const char* value = "0x4a0";

    cJSON_AddStringToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, 0x4a0);
}

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
// C23 及以后，strtoxxx 函数已原生支持 0b 前缀
void
test_cvt_json_binary_string_to_single_int32(void)
{
    const char* value = "0b110";

    cJSON_AddStringToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32 == 0b110);
}
#endif

void
test_cvt_json_octal_string_to_single_int32(void)
{
    const char* value = "0110";

    cJSON_AddStringToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, 0110);
}

void
test_cvt_json_number_to_oneof_int32(void)
{
    int32_t oneof_int32_value = 123456789;
    cJSON_AddNumberToObject(root, "oneof_int32", oneof_int32_value);

    j2p_expt_t                      e          = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, "oneof_int32"), (ProtobufCMessage*)msg, "oneof_int32");
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_int32");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_int32, oneof_int32_value);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

void
test_cvt_json_number_overflow_int32(void)
{
    int64_t value = (int64_t)INT32_MAX + 1;

    cJSON_AddNumberToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_number_underflow_int32(void)
{
    int64_t value = (int64_t)INT32_MIN - 1;

    cJSON_AddNumberToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_str_overflow_int32(void)
{
    const char* value = "2147483648";

    cJSON_AddStringToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_str_underflow_int32(void)
{
    const char* value = "-0x80000001";

    cJSON_AddStringToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_number_max_int32(void)
{
    int32_t value = INT32_MAX;

    cJSON_AddNumberToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, value);
}

void
test_cvt_json_number_min_int32(void)
{
    int32_t value = INT32_MIN;

    cJSON_AddNumberToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, value);
}

void
test_cvt_json_str_max_int32(void)
{
    const char* value = "0x7fffffff";

    cJSON_AddStringToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, INT32_MAX);
}

void
test_cvt_json_str_min_int32(void)
{
    const char* value = "-0x80000000";

    cJSON_AddStringToObject(root, int32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int32_field_name), (ProtobufCMessage*)msg, int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, INT32_MIN);
}

void
test_cvt_json_array_to_repeated_int32(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    const char*   value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0b110\",\"0110\", 123.45]";
    const int32_t expect_array[] = {1234567890, 1234567890, 0x4a0, 0b110, 0110, 123};
#else
    const char*   value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0110\", 123.45]";
    const int32_t expect_array[] = {1234567890, 1234567890, 0x4a0, 0110, 123};
#endif
    const size_t expect_length = sizeof(expect_array) / sizeof(int32_t);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_int32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int32_field_name), (ProtobufCMessage*)msg, repeated_int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_int32, expect_array, expect_length * sizeof(int32_t)), 0);
}

void
test_cvt_json_array_to_repeated_int32_partly_failed(void)
{
    const char*   value          = "[1234567890, \"0x4a0\",\"0110\",\"invalid number string\",\"0x80000000\"]";
    const int32_t expect_array[] = {1234567890, 0x4a0, 0110};
    const size_t  expect_length  = sizeof(expect_array) / sizeof(int32_t);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_int32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int32_field_name), (ProtobufCMessage*)msg, repeated_int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_PARTIAL_FAIL);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_int32, expect_array, expect_length * sizeof(int32_t)), 0);
}

void
test_cvt_json_array_to_repeated_int32_all_failed(void)
{
    const char* value = "[\"invalid number string\",\"0x80000000\"]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_int32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int32_field_name), (ProtobufCMessage*)msg, repeated_int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NO_VALID_FOUND);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
}

void
test_cvt_json_array_to_repeated_int32_empty(void)
{
    const char* value = "[]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_int32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int32_field_name), (ProtobufCMessage*)msg, repeated_int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_EMPTY_ARRAY);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
}

void
test_cvt_json_bool_to_repeated_int32(void)
{
    cJSON_AddTrueToObject(root, repeated_int32_field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int32_field_name), (ProtobufCMessage*)msg, repeated_int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
}

void
test_cvt_json_number_to_repeated_int32(void)
{
    cJSON_AddNumberToObject(root, repeated_int32_field_name, 1234);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int32_field_name), (ProtobufCMessage*)msg, repeated_int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
}

void
test_cvt_json_string_to_repeated_int32(void)
{
    cJSON_AddStringToObject(root, repeated_int32_field_name, "invalid repeated string");

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int32_field_name), (ProtobufCMessage*)msg, repeated_int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
}

void
test_cvt_json_object_to_repeated_int32(void)
{
    cJSON_AddObjectToObject(root, repeated_int32_field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int32_field_name), (ProtobufCMessage*)msg, repeated_int32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
}

void
test_cvt_json_number_to_sint32(void)
{
    int32_t value = 0x4a0;

    cJSON_AddNumberToObject(root, sint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, sint32_field_name), (ProtobufCMessage*)msg, sint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_sint32, value);
}

void
test_cvt_json_array_to_repeated_sint32(void)
{
    const char* value          = "[123,\"-123\",\"0x4a0\",\"0110\"]";
    int32_t     expect_array[] = {123, -123, 0x4a0, 0110};
    size_t      expect_length  = sizeof(expect_array) / sizeof(int32_t);

    cJSON* array_value = cJSON_Parse(value);

    cJSON_AddItemToObject(root, repeated_sint32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_sint32_field_name), (ProtobufCMessage*)msg, repeated_sint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_sint32, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_sint32, expect_array, expect_length * sizeof(int32_t)), 0);
}

void
test_cvt_json_number_to_sfixed32(void)
{
    int32_t value = 0x4a0;

    cJSON_AddNumberToObject(root, sfixed32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, sfixed32_field_name), (ProtobufCMessage*)msg, sfixed32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_sfixed32, value);
}

void
test_cvt_json_array_to_repeated_sfixed32(void)
{
    const char* value          = "[123,\"-123\",\"0x4a0\",\"0110\"]";
    int32_t     expect_array[] = {123, -123, 0x4a0, 0110};
    size_t      expect_length  = sizeof(expect_array) / sizeof(int32_t);

    cJSON* array_value = cJSON_Parse(value);

    cJSON_AddItemToObject(root, repeated_sfixed32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_sfixed32_field_name), (ProtobufCMessage*)msg, repeated_sfixed32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_sfixed32, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_sfixed32, expect_array, expect_length * sizeof(int32_t)), 0);
}

/******************************************************************************/
/*                                 Main Code                                  */
/******************************************************************************/

int
main(int argc, char const* argv[])
{
    init_file_name(__FILE__);
    unsigned  rv    = 1;
    CU_pSuite suite = NULL;

    if (CUE_SUCCESS == CU_initialize_registry() && CUE_SUCCESS == CU_register_suites(suites)) {
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_basic_show_failures(CU_get_failure_list());
        rv = CU_get_number_of_tests_failed();
        CU_cleanup_registry();
    }

    if (0 != rv) {
        exit(EXIT_FAILURE);
    }
    return 0;
}
