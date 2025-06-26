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

void test_cvt_json_number_to_single_int32_t(void);
void test_cvt_json_decimal_string_to_single_int32_t(void);
void test_cvt_json_hex_string_to_single_int32_t(void);
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void test_cvt_json_binary_string_to_single_int32_t(void);
#endif
void test_cvt_json_octal_string_to_single_int32_t(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

TestMessage* msg                     = NULL;
cJSON*       root                    = NULL;
CU_TestInfo  test_int32_conversion[] = {
    {"convert json number to protobuf int32 field",         test_cvt_json_number_to_single_int32_t        },
    {"convert json decimal string to protobuf int32 field", test_cvt_json_decimal_string_to_single_int32_t},
    {"convert json hex string to protobuf int32 field",     test_cvt_json_hex_string_to_single_int32_t    },
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    {"convert json binary string to protobuf int32 field",  test_cvt_json_binary_string_to_single_int32_t },
#endif
    {"convert json octal string to protobuf int32 field",   test_cvt_json_octal_string_to_single_int32_t  },
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"convert json to protobuf int32 field", NULL, NULL, setup_successful_conversion, teardown_successful_conversion, test_int32_conversion},
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
test_cvt_json_number_to_single_int32_t(void)
{
    int32_t     value      = 1234567890;
    const char* field_name = "f_int32";

    cJSON_AddNumberToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_TRUE(e == J2P_EXPT_SUCCESS);
    CU_ASSERT_TRUE(msg->f_int32 == value);
}

void
test_cvt_json_decimal_string_to_single_int32_t(void)
{
    const char* value      = "1234567890";
    const char* field_name = "f_int32";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_TRUE(e == J2P_EXPT_SUCCESS);
    CU_ASSERT_TRUE(msg->f_int32 == strtoull(value, NULL, 10));
}

void
test_cvt_json_hex_string_to_single_int32_t(void)
{
    const char* value      = "0x4a0";
    const char* field_name = "f_int32";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_TRUE(e == J2P_EXPT_SUCCESS);
    CU_ASSERT_TRUE(msg->f_int32 == strtoull(value, NULL, 16));
}

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
// C23 及以后，strtoxxx 函数已原生支持 0b 前缀
void
test_cvt_json_binary_string_to_single_int32_t(void)
{
    const char* value      = "0b110";
    const char* field_name = "f_int32";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_TRUE(e == J2P_EXPT_SUCCESS);
    CU_ASSERT_TRUE(msg->f_int32 == strtoll(value, NULL, 2));
}
#endif

void
test_cvt_json_octal_string_to_single_int32_t(void)
{
    const char* value      = "0110";
    const char* field_name = "f_int32";

    cJSON_AddStringToObject(root, field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
    CU_ASSERT_TRUE(e == J2P_EXPT_SUCCESS);
    CU_ASSERT_TRUE(msg->f_int32 == strtoull(value, NULL, 8));
}

void
test_cvt_single_int32_t_overflow(void)
{
    {
        int64_t     value      = (int64_t)INT32_MAX + 1;
        const char* field_name = "f_int32";

        cJSON* root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, field_name, value);

        TestMessage* msg = (TestMessage*)calloc(1, sizeof(TestMessage));
        test_message__init(msg);

        j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
        CU_ASSERT_TRUE(e == J2P_EXPT_VALUE_OVERFLOW);
        CU_ASSERT_TRUE(msg->f_int32 == 0);

        cJSON_Delete(root);

        test_message__free_unpacked(msg, NULL);
    };

    {
        int64_t     value      = (int64_t)INT32_MIN - 1;
        const char* field_name = "f_int32";

        cJSON* root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, field_name, value);

        TestMessage* msg = (TestMessage*)calloc(1, sizeof(TestMessage));
        test_message__init(msg);

        j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, field_name), (ProtobufCMessage*)msg, field_name);
        CU_ASSERT_TRUE(e == J2P_EXPT_VALUE_OVERFLOW);
        CU_ASSERT_TRUE(msg->f_int32 == 0);

        cJSON_Delete(root);

        test_message__free_unpacked(msg, NULL);
    };
}

void
test_cvt_repeated_int32_t_success(void)
{
}

void
test_incorrect_operation(void)
{
    TestMessage* msg = (TestMessage*)calloc(1, sizeof(TestMessage));
    j2p_expt_t   e   = cvt_json_2_pb_field(NULL, NULL, NULL, NULL, NULL, NULL, J2P_FILE_HEX_STR);
    CU_ASSERT_TRUE(e == J2P_EXPT_INVALID_ARG);
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
