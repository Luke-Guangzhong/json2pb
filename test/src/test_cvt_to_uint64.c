/**
 * @file test_cvt_to_uint64.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-07-01
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

void test_cvt_json_number_to_single_uint64(void);
void test_cvt_json_decimal_string_to_single_uint64(void);
void test_cvt_json_hex_string_to_single_uint64(void);
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void test_cvt_json_binary_string_to_single_uint64(void);
#endif
void test_cvt_json_octal_string_to_single_uint64(void);
void test_cvt_json_number_to_oneof_uint64(void);
void test_cvt_json_bool_to_uint64(void);
void test_cvt_json_array_to_uint64(void);
void test_cvt_json_object_to_uint64(void);

void test_cvt_json_number_overflow_uint64(void);
void test_cvt_json_number_underflow_uint64(void);
void test_cvt_json_str_overflow_uint64(void);
void test_cvt_json_str_underflow_uint64(void);
void test_cvt_json_number_max_uint64(void);
void test_cvt_json_number_min_uint64(void);
void test_cvt_json_str_max_uint64(void);
void test_cvt_json_str_min_uint64(void);
void test_cvt_json_inf_to_uint64(void);
void test_cvt_json_nan_to_uint64(void);

void test_cvt_json_array_to_repeated_uint64(void);
void test_cvt_json_array_to_repeated_uint64_partly_failed(void);
void test_cvt_json_array_to_repeated_uint64_all_failed(void);
void test_cvt_json_array_to_repeated_uint64_empty(void);

void test_cvt_json_number_to_fixed64(void);
void test_cvt_json_array_to_repeated_fixed64(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char uint64_field_name[]           = "f_uint64";
const char fixed64_field_name[]          = "f_fixed64";
const char repeated_uint64_field_name[]  = "f_repeated_uint64";
const char repeated_fixed64_field_name[] = "f_repeated_fixed64";

CU_TestInfo test_uint64_conversion[] = {
    {"Convert JSON number to Protobuf uint64 field",              test_cvt_json_number_to_single_uint64        },
    {"Convert JSON decimal string to Protobuf uint64 field",      test_cvt_json_decimal_string_to_single_uint64},
    {"Convert JSON hexadecimal string to Protobuf uint64 field",  test_cvt_json_hex_string_to_single_uint64    },
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    {"Convert JSON binary string to Protobuf uint64 field",       test_cvt_json_binary_string_to_single_uint64 },
#endif
    {"Convert JSON octal string to Protobuf uint64 field",        test_cvt_json_octal_string_to_single_uint64  },
    {"Convert JSON number to Protobuf oneof uint64 field",        test_cvt_json_number_to_oneof_uint64         },
    {"Reject conversion of JSON bool to Protobuf uint64 field",   test_cvt_json_bool_to_uint64                 },
    {"Reject conversion of JSON array to Protobuf uint64 field",  test_cvt_json_array_to_uint64                },
    {"Reject conversion of JSON object to Protobuf uint64 field", test_cvt_json_object_to_uint64               },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_uint64_overflow[] = {
    {"Reject conversion of JSON number to Protobuf uint64 on overflow",       test_cvt_json_number_overflow_uint64 },
    {"Reject conversion of JSON number to Protobuf uint64 on underflow",      test_cvt_json_number_underflow_uint64},
    {"Reject conversion of JSON string to Protobuf uint64 on overflow",       test_cvt_json_str_overflow_uint64    },
    {"Reject conversion of JSON string to Protobuf uint64 on underflow",      test_cvt_json_str_underflow_uint64   },
    {"Reject conversion of invalid cjson(inf) to protobuf int64 on overflow", test_cvt_json_inf_to_uint64          },
    {"Reject conversion of invalid cjson(nan) to protobuf int64 on overflow", test_cvt_json_nan_to_uint64          },
    {"Accept conversion of JSON number to Protobuf uint64 at MAX_EXACT",      test_cvt_json_number_max_uint64      },
    {"Accept conversion of JSON number to Protobuf uint64 at MIN_EXACT",      test_cvt_json_number_min_uint64      },
    {"Accept conversion of JSON string to Protobuf uint64 at UINT64_MAX",     test_cvt_json_str_max_uint64         },
    {"Accept conversion of JSON string to Protobuf uint64 at UINT64_MIN",     test_cvt_json_str_min_uint64         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_uint64_conversion[] = {
    {"Convert JSON array to repeated Protobuf uint64 field",              test_cvt_json_array_to_repeated_uint64              },
    {"Partial failure converting JSON array to repeated Protobuf uint64", test_cvt_json_array_to_repeated_uint64_partly_failed},
    {"All failure converting JSON array to repeated Protobuf uint64",     test_cvt_json_array_to_repeated_uint64_all_failed   },
    {"Convert Empty JSON array to repeated Protobuf uint64",              test_cvt_json_array_to_repeated_uint64_empty        },
    CU_TEST_INFO_NULL
};

CU_TestInfo test_xu64_conversion[] = {
    {"Convert JSON number to Protobuf fixed64 field",         test_cvt_json_number_to_fixed64        },
    {"Convert JSON array to Protobuf repeated fixed64 field", test_cvt_json_array_to_repeated_fixed64},
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf uint64 field",                  init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_uint64_conversion},
    {"Convert JSON to Protobuf uint64 with overflow handling", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_uint64_overflow                                                                                                                                                              },
    {"Convert JSON array to repeated Protobuf uint64 field",   init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_repeated_uint64_conversion                                                                                                                                                   },
    {"Convert JSON to Protobuf fixed64 field",                 init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_xu64_conversion  },
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_number_to_single_uint64(void)
{
    uint64_t value = 1234567890;

    cJSON_AddNumberToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64, value);
}

void
test_cvt_json_decimal_string_to_single_uint64(void)
{
    const char* value = "1234567890";

    cJSON_AddStringToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64, 1234567890);
}

void
test_cvt_json_hex_string_to_single_uint64(void)
{
    const char* value = "0x4a0";

    cJSON_AddStringToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64, 0x4a0);
}

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void
test_cvt_json_binary_string_to_single_uint64(void)
{
    const char* value = "0b110";

    cJSON_AddStringToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64 == 0b110);
}
#endif

void
test_cvt_json_octal_string_to_single_uint64(void)
{
    const char* value = "0110";

    cJSON_AddStringToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64, 0110);
}

void
test_cvt_json_number_to_oneof_uint64(void)
{
    uint64_t oneof_uint64_value = 123456789;
    cJSON_AddNumberToObject(root, "oneof_uint64", oneof_uint64_value);

    j2p_expt_t                      e          = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, "oneof_uint64"), (ProtobufCMessage*)msg, "oneof_uint64");
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_uint64");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_uint64, oneof_uint64_value);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

void
test_cvt_json_bool_to_uint64(void)
{
    cJSON_AddTrueToObject(root, uint64_field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_array_to_uint64(void)
{
    cJSON_AddArrayToObject(root, uint64_field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_object_to_uint64(void)
{
    cJSON_AddObjectToObject(root, uint64_field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_number_overflow_uint64(void)
{
    double value = 1.0e+100;

    cJSON_AddNumberToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NOT_EXACT);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_number_underflow_uint64(void)
{
    double value = -1;

    cJSON_AddNumberToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NOT_EXACT);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_str_overflow_uint64(void)
{
    const char* value = "18446744073709551616";

    cJSON_AddStringToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_str_underflow_uint64(void)
{
    const char* value = "-10";

    cJSON_AddStringToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_number_max_uint64(void)
{
    uint64_t value = 0x1.0p53;

    cJSON_AddNumberToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64, value);
}

void
test_cvt_json_number_min_uint64(void)
{
    uint64_t value = 0;

    cJSON_AddNumberToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64, value);
}

void
test_cvt_json_str_max_uint64(void)
{
    const char* value = "18446744073709551615";

    cJSON_AddStringToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64, UINT64_MAX);
}

void
test_cvt_json_str_min_uint64(void)
{
    const char* value = "0";

    cJSON_AddStringToObject(root, uint64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_inf_to_uint64(void)
{
    cJSON_AddNumberToObject(root, uint64_field_name, INFINITY);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_nan_to_uint64(void)
{
    cJSON_AddNumberToObject(root, uint64_field_name, NAN);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint64_field_name), (ProtobufCMessage*)msg, uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_uint64, 0);
}

void
test_cvt_json_array_to_repeated_uint64(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    const char*    value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0b110\",\"0110\",123.45]";
    const uint64_t expect_array[] = {1234567890, 1234567890, 0x4a0, 0b110, 0110, 123};
#else
    const char*    value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0110\",123.45]";
    const uint64_t expect_array[] = {1234567890, 1234567890, 0x4a0, 0110, 123};
#endif
    const size_t expect_length = sizeof(expect_array) / sizeof(uint64_t);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_uint64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_uint64_field_name), (ProtobufCMessage*)msg, repeated_uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_uint64, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_uint64, expect_array, expect_length * sizeof(uint64_t)), 0);
}

void
test_cvt_json_array_to_repeated_uint64_partly_failed(void)
{
    const char*    value          = "[1234567890, \"0x4a0\",\"0110\",\"invalid number string\",\"0x10000000000000000\"]";
    const uint64_t expect_array[] = {1234567890, 0x4a0, 0110};
    const size_t   expect_length  = sizeof(expect_array) / sizeof(uint64_t);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_uint64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_uint64_field_name), (ProtobufCMessage*)msg, repeated_uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_PARTIAL_FAIL);
    CU_ASSERT_EQUAL(msg->n_f_repeated_uint64, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_uint64, expect_array, expect_length * sizeof(uint64_t)), 0);
}

void
test_cvt_json_array_to_repeated_uint64_all_failed(void)
{
    const char* value = "[\"invalid number string\",\"0x10000000000000000\"]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_uint64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_uint64_field_name), (ProtobufCMessage*)msg, repeated_uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NO_VALID_FOUND);
    CU_ASSERT_EQUAL(msg->n_f_repeated_uint64, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_uint64);
}

void
test_cvt_json_array_to_repeated_uint64_empty(void)
{
    const char* value = "[]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_uint64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_uint64_field_name), (ProtobufCMessage*)msg, repeated_uint64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_EMPTY_ARRAY);
    CU_ASSERT_EQUAL(msg->n_f_repeated_uint64, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_uint64);
}

void
test_cvt_json_number_to_fixed64(void)
{
    uint64_t value = 0x4a0;

    cJSON_AddNumberToObject(root, fixed64_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, fixed64_field_name), (ProtobufCMessage*)msg, fixed64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_fixed64, value);
}

void
test_cvt_json_array_to_repeated_fixed64(void)
{
    const char* value          = "[123,\"123\",\"0x4a0\",\"0110\"]";
    uint64_t    expect_array[] = {123, 123, 0x4a0, 0110};
    size_t      expect_length  = sizeof(expect_array) / sizeof(uint64_t);

    cJSON* array_value = cJSON_Parse(value);

    cJSON_AddItemToObject(root, repeated_fixed64_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_fixed64_field_name), (ProtobufCMessage*)msg, repeated_fixed64_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_fixed64, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_fixed64, expect_array, expect_length * sizeof(uint64_t)), 0);
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
