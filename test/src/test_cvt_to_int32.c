/**
 * @file test_cvt_int32_t.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdlib.h>

#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>

#include "json2pb.h"
#include "test.pb-c.h"

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

void setup_successful_conversion(void);
void teardown_successful_conversion(void);

void test_cvt_json_number_to_single_int32(void);
void test_cvt_json_decimal_string_to_single_int32(void);
void test_cvt_json_hex_string_to_single_int32(void);
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void test_cvt_json_binary_string_to_single_int32(void);
#endif
void test_cvt_json_octal_string_to_single_int32(void);

void test_cvt_json_number_overflow_int32(void);
void test_cvt_json_number_underflow_int32(void);
void test_cvt_json_str_overflow_int32(void);
void test_cvt_json_str_underflow_int32(void);
void test_cvt_json_number_max_int32(void);
void test_cvt_json_number_min_int32(void);
void test_cvt_json_str_max_int32(void);
void test_cvt_json_str_min_int32(void);

void test_cvt_json_null_to_single_number(void);
void test_cvt_json_bool_to_single_number(void);
void test_cvt_json_array_to_single_number(void);
void test_cvt_json_object_to_single_number(void);
void test_cvt_invalid_json_str_to_single_number(void);

void test_cvt_json_array_to_repeated_int32(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

TestMessage* msg                 = NULL;
cJSON*       root                = NULL;
const char*  field_name          = "f_int32";
const char*  repeated_field_name = "f_repeated_int32";

CU_TestInfo test_int32_conversion[] = {
    {"convert json number to protobuf int32 field",         test_cvt_json_number_to_single_int32        },
    {"convert json decimal string to protobuf int32 field", test_cvt_json_decimal_string_to_single_int32},
    {"convert json hex string to protobuf int32 field",     test_cvt_json_hex_string_to_single_int32    },
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    {"convert json binary string to protobuf int32 field",  test_cvt_json_binary_string_to_single_int32 },
#endif
    {"convert json octal string to protobuf int32 field",   test_cvt_json_octal_string_to_single_int32  },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_int32_overflow[] = {
    {"convert json number overflow",  test_cvt_json_number_overflow_int32 },
    {"convert json number underflow", test_cvt_json_number_underflow_int32},
    {"convert json string overflow",  test_cvt_json_str_overflow_int32    },
    {"convert json string underflow", test_cvt_json_str_underflow_int32   },
    {"convert json number max",       test_cvt_json_number_max_int32      },
    {"convert json number min",       test_cvt_json_number_min_int32      },
    {"convert json string max",       test_cvt_json_str_max_int32         },
    {"convert json string min",       test_cvt_json_str_min_int32         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_int32_invalid_json_type[] = {
    {"convert json null to int32",           test_cvt_json_null_to_single_number       },
    {"convert json bool to int32",           test_cvt_json_bool_to_single_number       },
    {"convert json array to int32",          test_cvt_json_array_to_single_number      },
    {"convert json object to int32",         test_cvt_json_object_to_single_number     },
    {"convert json invalid string to int32", test_cvt_invalid_json_str_to_single_number},
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"convert json to protobuf int32 field", NULL, NULL, setup_successful_conversion, teardown_successful_conversion, test_int32_conversion       },
    {"convert json to int32 overflow",       NULL, NULL, setup_successful_conversion, teardown_successful_conversion, test_int32_overflow         },
    {"convert invalid json to int32",        NULL, NULL, setup_successful_conversion, teardown_successful_conversion, test_int32_invalid_json_type},
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
setup_successful_conversion(void)
{
    msg = (TestMessage*)calloc(1, sizeof(TestMessage));
    if (NULL == msg) {
        exit(EXIT_FAILURE);
    }
    test_message__init(msg);
    root = cJSON_CreateObject();
    if (NULL == root) {
        exit(EXIT_FAILURE);
    }
    return;
}

void
teardown_successful_conversion(void)
{
    cJSON_Delete(root);
    test_message__free_unpacked(msg, NULL);
    msg  = NULL;
    root = NULL;
    return;
}

void
test_cvt_json_number_to_single_int32(void)
{
    int32_t value = 1234567890;

    cJSON_AddNumberToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, value);
}

void
test_cvt_json_decimal_string_to_single_int32(void)
{
    const char* value = "1234567890";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, 1234567890);
}

void
test_cvt_json_hex_string_to_single_int32(void)
{
    const char* value = "0x4a0";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, 0x4a0);
}

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
// C23 及以后，strtoxxx 函数已原生支持 0b 前缀
void
test_cvt_json_binary_string_to_single_int32(void)
{
    const char* value = "0b110";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32 == 0b110);
}
#endif

void
test_cvt_json_octal_string_to_single_int32(void)
{
    const char* value = "0110";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, 0110);
}

void
test_cvt_json_number_overflow_int32(void)
{
    int64_t value = (int64_t)INT32_MAX + 1;

    cJSON_AddNumberToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_number_underflow_int32(void)
{
    int64_t value = (int64_t)INT32_MIN - 1;

    cJSON_AddNumberToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_str_overflow_int32(void)
{
    const char* value = "0x80000000";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_str_underflow_int32(void)
{
    const char* value = "-0x80000001";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_number_max_int32(void)
{
    int32_t value = INT32_MAX;

    cJSON_AddNumberToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, value);
}

void
test_cvt_json_number_min_int32(void)
{
    int32_t value = INT32_MIN;

    cJSON_AddNumberToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, value);
}

void
test_cvt_json_str_max_int32(void)
{
    const char* value = "0x7fffffff";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, INT32_MAX);
}

void
test_cvt_json_str_min_int32(void)
{
    const char* value = "-0x80000000";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_int32, INT32_MIN);
}

void
test_cvt_json_null_to_single_number(void)
{
    cJSON_AddNullToObject(root, field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NULL_VALUE);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_bool_to_single_number(void)
{
    cJSON_AddTrueToObject(root, field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_array_to_single_number(void)
{

    cJSON_AddArrayToObject(root, field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_object_to_single_number(void)
{

    cJSON_AddObjectToObject(root, field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_invalid_json_str_to_single_number(void)
{
    const char* value = "0x110q4sslkdfjalsdf";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_NUMBER_STRING);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_cvt_json_array_to_repeated_int32(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    const char* value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0b110\",\"0110\"]";
    int32_t     res_value[]    = {1234567890, 1234567890, 0x4a0, 0b110, 0110};
    size_t      valid_elem_num = 5;
#else
    const char* value          = "[1234567890, \"1234567890\",\"0x4a0\",\"0110\"]";
    int32_t     res_value[]    = {1234567890, 1234567890, 0x4a0, 0110};
    size_t      valid_elem_num = 4;
#endif

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_field_name), (ProtobufCMessage*)msg, repeated_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, valid_elem_num);
    memcmp(msg->f_repeated_int32, res_value, valid_elem_num * sizeof(int32_t));
}

/******************************************************************************/
/*                                 Main Code                                  */
/******************************************************************************/

int
main(int argc, char const* argv[])
{
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
