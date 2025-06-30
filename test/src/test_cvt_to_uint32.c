/**
 * @file test_cvt_to_uint32.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-30
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

void test_cvt_json_number_to_single_uint32(void);
void test_cvt_json_decimal_string_to_single_uint32(void);
void test_cvt_json_hex_string_to_single_uint32(void);
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void test_cvt_json_binary_string_to_single_uint32(void);
#endif
void test_cvt_json_octal_string_to_single_uint32(void);
void test_cvt_json_number_to_oneof_uint32(void);

void test_cvt_json_number_overflow_uint32(void);
void test_cvt_json_number_underflow_uint32(void);
void test_cvt_json_str_overflow_uint32(void);
void test_cvt_json_str_underflow_uint32(void);
void test_cvt_json_number_max_uint32(void);
void test_cvt_json_number_min_uint32(void);
void test_cvt_json_str_max_uint32(void);
void test_cvt_json_str_min_uint32(void);

void test_cvt_json_array_to_repeated_uint32(void);
void test_cvt_json_array_to_repeated_uint32_partly_failed(void);
void test_cvt_json_array_to_repeated_uint32_all_failed(void);
void test_cvt_json_array_to_repeated_uint32_empty(void);

void test_cvt_json_number_to_fixed32(void);
void test_cvt_json_array_to_repeated_fixed32(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char uint32_field_name[]           = "f_uint32";
const char fixed32_field_name[]          = "f_fixed32";
const char repeated_uint32_field_name[]  = "f_repeated_uint32";
const char repeated_fixed32_field_name[] = "f_repeated_fixed32";

CU_TestInfo test_uint32_conversion[] = {
    {"Convert JSON number to Protobuf int64 field",             test_cvt_json_number_to_single_uint32        },
    {"Convert JSON decimal string to Protobuf int64 field",     test_cvt_json_decimal_string_to_single_uint32},
    {"Convert JSON hexadecimal string to Protobuf int64 field", test_cvt_json_hex_string_to_single_uint32    },
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    {"Convert JSON binary string to Protobuf int64 field",      test_cvt_json_binary_string_to_single_uint32 },
#endif
    {"Convert JSON octal string to Protobuf int64 field",       test_cvt_json_octal_string_to_single_uint32  },
    {"Convert JSON number to Protobuf oneof int64 field",       test_cvt_json_number_to_oneof_uint32         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_uint32_overflow[] = {
    {"Reject conversion of JSON number to Protobuf uint32 on overflow",   test_cvt_json_number_overflow_uint32 },
    {"Reject conversion of JSON number to Protobuf uint32 on underflow",  test_cvt_json_number_underflow_uint32},
    {"Reject conversion of JSON string to Protobuf uint32 on overflow",   test_cvt_json_str_overflow_uint32    },
    {"Reject conversion of JSON string to Protobuf uint32 on underflow",  test_cvt_json_str_underflow_uint32   },
    {"Accept conversion of JSON number to Protobuf uint32 at UINT32_MAX", test_cvt_json_number_max_uint32      },
    {"Accept conversion of JSON number to Protobuf uint32 at UINT32_MIN", test_cvt_json_number_min_uint32      },
    {"Accept conversion of JSON string to Protobuf uint32 at UINT32_MAX", test_cvt_json_str_max_uint32         },
    {"Accept conversion of JSON string to Protobuf uint32 at UINT32_MIN", test_cvt_json_str_min_uint32         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_uint32_conversion[] = {
    {"Convert JSON array to repeated Protobuf uint32 field",              test_cvt_json_array_to_repeated_uint32              },
    {"Partial failure converting JSON array to repeated Protobuf uint32", test_cvt_json_array_to_repeated_uint32_partly_failed},
    {"All failure converting JSON array to repeated Protobuf uint32",     test_cvt_json_array_to_repeated_uint32_all_failed   },
    {"Convert Empty JSON array to repeated Protobuf uint32",              test_cvt_json_array_to_repeated_uint32_empty        },
    CU_TEST_INFO_NULL
};

CU_TestInfo test_xu32_conversion[] = {
    {"Convert JSON number to Protobuf fixed32 field",         test_cvt_json_number_to_fixed32        },
    {"Convert JSON array to Protobuf repeated fixed32 field", test_cvt_json_array_to_repeated_fixed32},
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf uint32 field",                  init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_uint32_conversion},
    {"Convert JSON to Protobuf uint32 with overflow handling", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_uint32_overflow                                                                                                                                                              },
    {"Convert JSON array to repeated Protobuf uint32 field",   init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_repeated_uint32_conversion                                                                                                                                                   },
    {"Convert JSON to Protobuf fixed32 field",                 init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_xu32_conversion  },
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_number_to_single_uint32(void)
{
    uint32_t value = 1234567890;

    cJSON_AddNumberToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32, value);
}

void
test_cvt_json_decimal_string_to_single_uint32(void)
{
    const char* value = "1234567890";

    cJSON_AddStringToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32, 1234567890);
}

void
test_cvt_json_hex_string_to_single_uint32(void)
{
    const char* value = "0x4a0";

    cJSON_AddStringToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32, 0x4a0);
}

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void
test_cvt_json_binary_string_to_single_uint32(void)
{
    const char* value = "0b110";

    cJSON_AddStringToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32 == 0b110);
}
#endif

void
test_cvt_json_octal_string_to_single_uint32(void)
{
    const char* value = "0110";

    cJSON_AddStringToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32, 0110);
}

void
test_cvt_json_number_to_oneof_uint32(void)
{
    uint32_t oneof_uint32_value = 123456789;
    cJSON_AddNumberToObject(root, "oneof_uint32", oneof_uint32_value);

    j2p_expt_t                      e          = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, "oneof_uint32"), (ProtobufCMessage*)msg, "oneof_uint32");
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_uint32");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_uint32, oneof_uint32_value);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

void
test_cvt_json_number_overflow_uint32(void)
{
    uint64_t value = (uint64_t)UINT32_MAX + 1;

    cJSON_AddNumberToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_uint32, 0);
}

void
test_cvt_json_number_underflow_uint32(void)
{
    int64_t value = (int64_t)-1;

    cJSON_AddNumberToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_uint32, 0);
}

void
test_cvt_json_str_overflow_uint32(void)
{
    const char* value = "4294967296";

    cJSON_AddStringToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_uint32, 0);
}

void
test_cvt_json_str_underflow_uint32(void)
{
    const char* value = "-1";

    cJSON_AddStringToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_uint32, 0);
}

void
test_cvt_json_number_max_uint32(void)
{
    uint32_t value = UINT32_MAX;

    cJSON_AddNumberToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32, value);
}

void
test_cvt_json_number_min_uint32(void)
{
    uint32_t value = 0;

    cJSON_AddNumberToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32, value);
}

void
test_cvt_json_str_max_uint32(void)
{
    const char* value = "4294967295";

    cJSON_AddStringToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32, UINT32_MAX);
}

void
test_cvt_json_str_min_uint32(void)
{
    const char* value = "0";

    cJSON_AddStringToObject(root, uint32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, uint32_field_name), (ProtobufCMessage*)msg, uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_uint32, 0);
}

void
test_cvt_json_array_to_repeated_uint32(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    const char*   value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0b110\",\"0110\"]";
    const int32_t res_value[]    = {1234567890, 1234567890, 0x4a0, 0b110, 0110};
    const size_t  valid_elem_num = 5;
#else
    const char*   value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0110\"]";
    const int32_t res_value[]    = {1234567890, 1234567890, 0x4a0, 0110};
    const size_t  valid_elem_num = 4;
#endif

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_uint32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_uint32_field_name), (ProtobufCMessage*)msg, repeated_uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_uint32, valid_elem_num);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_uint32, res_value, valid_elem_num * sizeof(uint32_t)), 0);
}

void
test_cvt_json_array_to_repeated_uint32_partly_failed(void)
{
    const char*   value          = "[1234567890, \"0x4a0\",\"0110\",\"invalid number string\",\"0x100000000\"]";
    const int32_t res_value[]    = {1234567890, 0x4a0, 0110};
    const size_t  valid_elem_num = 3;

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_uint32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_uint32_field_name), (ProtobufCMessage*)msg, repeated_uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_PARTIAL_FAIL);
    CU_ASSERT_EQUAL(msg->n_f_repeated_uint32, valid_elem_num);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_uint32, res_value, valid_elem_num * sizeof(uint32_t)), 0);
}

void
test_cvt_json_array_to_repeated_uint32_all_failed(void)
{
    const char* value = "[\"invalid number string\",\"0x100000000\"]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_uint32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_uint32_field_name), (ProtobufCMessage*)msg, repeated_uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NO_VALID_FOUND);
    CU_ASSERT_EQUAL(msg->n_f_repeated_uint32, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_uint32);
}

void
test_cvt_json_array_to_repeated_uint32_empty(void)
{
    const char* value = "[]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_uint32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_uint32_field_name), (ProtobufCMessage*)msg, repeated_uint32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_EMPTY_ARRAY);
    CU_ASSERT_EQUAL(msg->n_f_repeated_uint32, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_uint32);
}

void
test_cvt_json_number_to_fixed32(void)
{
    uint32_t value = 0x4a0;

    cJSON_AddNumberToObject(root, fixed32_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, fixed32_field_name), (ProtobufCMessage*)msg, fixed32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_fixed32, value);
}

void
test_cvt_json_array_to_repeated_fixed32(void)
{
    const char* value          = "[123,\"123\",\"0x4a0\",\"0110\"]";
    uint32_t    expect_array[] = {123, 123, 0x4a0, 0110};
    size_t      expect_length  = 4;

    cJSON* array_value = cJSON_Parse(value);

    cJSON_AddItemToObject(root, repeated_fixed32_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_fixed32_field_name), (ProtobufCMessage*)msg, repeated_fixed32_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_fixed32, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_fixed32, expect_array, expect_length * sizeof(uint32_t)), 0);
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
