/**
 * @file test_cvt_to_int64.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-27
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

void test_cvt_json_number_to_single_int64(void);
void test_cvt_json_decimal_string_to_single_int64(void);
void test_cvt_json_hex_string_to_single_int64(void);
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void test_cvt_json_binary_string_to_single_int64(void);
#endif
void test_cvt_json_octal_string_to_single_int64(void);
void test_cvt_json_number_to_oneof_int64(void);

void test_cvt_json_number_overflow_int64(void);
void test_cvt_json_number_underflow_int64(void);
void test_cvt_json_str_overflow_int64(void);
void test_cvt_json_str_underflow_int64(void);
void test_cvt_json_number_max_int64(void);
void test_cvt_json_number_min_int64(void);
void test_cvt_json_str_max_int64(void);
void test_cvt_json_str_min_int64(void);

void test_cvt_json_array_to_repeated_int64(void);
void test_cvt_json_array_to_repeated_int64_partly_failed(void);
void test_cvt_json_array_to_repeated_int64_all_failed(void);
void test_cvt_json_array_to_repeated_int64_empty(void);

void test_cvt_json_number_to_sint64(void);
void test_cvt_json_array_to_repeated_sint64(void);
void test_cvt_json_number_to_sfixed64(void);
void test_cvt_json_array_to_repeated_sfixed64(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char int64_field_name[]             = "f_int64";
const char sint64_field_name[]            = "f_sint64";
const char sfixed64_field_name[]          = "f_sfixed64";
const char repeated_int64_field_name[]    = "f_repeated_int64";
const char repeated_sint64_field_name[]   = "f_repeated_sint64";
const char repeated_sfixed64_field_name[] = "f_repeated_sfixed64";

CU_TestInfo test_int64_conversion[] = {
    {"Convert JSON number to Protobuf int64 field",             test_cvt_json_number_to_single_int64        },
    {"Convert JSON decimal string to Protobuf int64 field",     test_cvt_json_decimal_string_to_single_int64},
    {"Convert JSON hexadecimal string to Protobuf int64 field", test_cvt_json_hex_string_to_single_int64    },
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    {"Convert JSON binary string to Protobuf int64 field",      test_cvt_json_binary_string_to_single_int64 },
#endif
    {"Convert JSON octal string to Protobuf int64 field",       test_cvt_json_octal_string_to_single_int64  },
    {"Convert JSON number to Protobuf oneof int64 field",       test_cvt_json_number_to_oneof_int64         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_int64_overflow[] = {
    {"Reject conversion of JSON number to Protobuf int64 on overflow",  test_cvt_json_number_overflow_int64 },
    {"Reject conversion of JSON number to Protobuf int64 on underflow", test_cvt_json_number_underflow_int64},
    {"Reject conversion of JSON string to Protobuf int64 on overflow",  test_cvt_json_str_overflow_int64    },
    {"Reject conversion of JSON string to Protobuf int64 on underflow", test_cvt_json_str_underflow_int64   },
    {"Accept conversion of JSON number to Protobuf int64 at EXACT_MAX", test_cvt_json_number_max_int64      },
    {"Accept conversion of JSON number to Protobuf int64 at EXACT_MIN", test_cvt_json_number_min_int64      },
    {"Accept conversion of JSON string to Protobuf int64 at INT64_MAX", test_cvt_json_str_max_int64         },
    {"Accept conversion of JSON string to Protobuf int64 at INT64_MIN", test_cvt_json_str_min_int64         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_int64_conversion[] = {
    {"Convert JSON array to repeated Protobuf int64 field",              test_cvt_json_array_to_repeated_int64              },
    {"Partial failure converting JSON array to repeated Protobuf int64", test_cvt_json_array_to_repeated_int64_partly_failed},
    {"All failure converting JSON array to repeated Protobuf int64",     test_cvt_json_array_to_repeated_int64_all_failed   },
    {"Convert Empty JSON array to repeated Protobuf int64",              test_cvt_json_array_to_repeated_int64_empty        },
    CU_TEST_INFO_NULL
};

CU_TestInfo test_x64_conversion[] = {
    {"Convert JSON number to Protobuf sint64 field",           test_cvt_json_number_to_sint64          },
    {"Convert JSON array to Protobuf repeated sint64 field",   test_cvt_json_array_to_repeated_sint64  },
    {"Convert JSON number to Protobuf sfixed64 field",         test_cvt_json_number_to_sfixed64        },
    {"Convert JSON array to Protobuf repeated sfixed64 field", test_cvt_json_array_to_repeated_sfixed64},
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf int64 field",                  init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_int64_conversion},
    {"Convert JSON to Protobuf int64 with overflow handling", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_int64_overflow                                                                                                                                                             },
    {"Convert JSON array to repeated Protobuf int64 field",   init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_repeated_int64_conversion                                                                                                                                                  },
    {"Convert JSON to Protobuf sint64/sfixed64 field",        init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_x64_conversion  },
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_number_to_single_int64(void)
{
    int64_t value = 1234567890;

    cJSON_AddNumberToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64, value);
}

void
test_cvt_json_decimal_string_to_single_int64(void)
{
    const char* value = "1234567890";

    cJSON_AddStringToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64, 1234567890);
}

void
test_cvt_json_hex_string_to_single_int64(void)
{
    const char* value = "0x4a0";

    cJSON_AddStringToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64, 0x4a0);
}

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void
test_cvt_json_binary_string_to_single_int64(void)
{
    const char* value = "0b110";

    cJSON_AddStringToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64 == 0b110);
}
#endif

void
test_cvt_json_octal_string_to_single_int64(void)
{
    const char* value = "0110";

    cJSON_AddStringToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64, 0110);
}

void
test_cvt_json_number_to_oneof_int64(void)
{
    int64_t oneof_int64_value = 123456789;
    cJSON_AddNumberToObject(root, "oneof_int64", oneof_int64_value);

    j2p_expt_t                      e          = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, "oneof_int64"), (ProtobufCMessage*)msg, "oneof_int64");
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_int64");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_int64, oneof_int64_value);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

void
test_cvt_json_number_overflow_int64(void)
{
    double value = 0x1.0p64;

    cJSON_AddNumberToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NOT_EXACT_INT64);
    CU_ASSERT_EQUAL(msg->f_int64, 0);
}

void
test_cvt_json_number_underflow_int64(void)
{
    double value = -0x1.0p64;

    cJSON_AddNumberToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NOT_EXACT_INT64);
    CU_ASSERT_EQUAL(msg->f_int64, 0);
}

void
test_cvt_json_str_overflow_int64(void)
{
    const char* value = "0x8000000000000000";

    cJSON_AddStringToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int64, 0);
}

void
test_cvt_json_str_underflow_int64(void)
{
    const char* value = "-0x8000000000000001";

    cJSON_AddStringToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int64, 0);
}

void
test_cvt_json_number_max_int64(void)
{
    double value = 0x1.0p53 - 1;

    cJSON_AddNumberToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64, 0x1.0p53 - 1);
}

void
test_cvt_json_number_min_int64(void)
{
    double value = -0x1.0p53 + 1;

    cJSON_AddNumberToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64, -0x1.0p53 + 1);
}

void
test_cvt_json_str_max_int64(void)
{
    const char* value = "0x7fffffffffffffff";

    cJSON_AddStringToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64, 0x7fffffffffffffff);
}

void
test_cvt_json_str_min_int64(void)
{
    const char* value = "-0x800000000000000";

    cJSON_AddStringToObject(root, int64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, int64_field_name), (ProtobufCMessage*)msg, int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int64, -0x800000000000000);
}

void
test_cvt_json_array_to_repeated_int64(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    const char*   value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0b110\",\"0110\"]";
    const int64_t res_value[]    = {1234567890, 1234567890, 0x4a0, 0b110, 0110};
    const size_t  valid_elem_num = 5;
#else
    const char*   value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0110\"]";
    const int64_t res_value[]    = {1234567890, 1234567890, 0x4a0, 0110};
    const size_t  valid_elem_num = 4;
#endif

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_int64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int64_field_name), (ProtobufCMessage*)msg, repeated_int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int64, valid_elem_num);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_int64, res_value, valid_elem_num * sizeof(int64_t)), 0);
}

void
test_cvt_json_array_to_repeated_int64_partly_failed(void)
{
    const char*   value          = "[1234567890, \"0x4a0\",\"0110\",\"invalid number string\",\"0x7fffffffffffffffffffffff\"]";
    const int64_t res_value[]    = {1234567890, 0x4a0, 0110};
    const size_t  valid_elem_num = 3;

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_int64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int64_field_name), (ProtobufCMessage*)msg, repeated_int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_PARTIAL_FAIL);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int64, valid_elem_num);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_int64, res_value, valid_elem_num * sizeof(int64_t)), 0);
}

void
test_cvt_json_array_to_repeated_int64_all_failed(void)
{
    const char* value = "[\"invalid number string\",\"0x7fffffffffffffffffffffff\"]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_int64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int64_field_name), (ProtobufCMessage*)msg, repeated_int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NO_VALID_FOUND);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int64, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int64);
}

void
test_cvt_json_array_to_repeated_int64_empty(void)
{
    const char* value = "[]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_int64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_int64_field_name), (ProtobufCMessage*)msg, repeated_int64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_EMPTY_ARRAY);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int64, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int64);
}

void
test_cvt_json_number_to_sint64(void)
{
    int64_t value = 0x4a0;

    cJSON_AddNumberToObject(root, sint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, sint64_field_name), (ProtobufCMessage*)msg, sint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_sint64, value);
}

void
test_cvt_json_array_to_repeated_sint64(void)
{
    const char* value          = "[123,\"123\",\"0x4a0\",\"0110\"]";
    int64_t     expect_array[] = {123, 123, 0x4a0, 0110};
    size_t      expect_length  = 4;

    cJSON* array_value = cJSON_Parse(value);

    cJSON_AddItemToObject(root, repeated_sint64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_sint64_field_name), (ProtobufCMessage*)msg, repeated_sint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_sint64, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_sint64, expect_array, expect_length * sizeof(int64_t)), 0);
}

void
test_cvt_json_number_to_sfixed64(void)
{
    int64_t value = 0x4a0;

    cJSON_AddNumberToObject(root, sfixed64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, sfixed64_field_name), (ProtobufCMessage*)msg, sfixed64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_sfixed64, value);
}

void
test_cvt_json_array_to_repeated_sfixed64(void)
{
    const char* value          = "[123,\"123\",\"0x4a0\",\"0110\"]";
    int64_t     expect_array[] = {123, 123, 0x4a0, 0110};
    size_t      expect_length  = 4;

    cJSON* array_value = cJSON_Parse(value);

    cJSON_AddItemToObject(root, repeated_sfixed64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_sfixed64_field_name), (ProtobufCMessage*)msg, repeated_sfixed64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_sfixed64, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_sfixed64, expect_array, expect_length * sizeof(int64_t)), 0);
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
