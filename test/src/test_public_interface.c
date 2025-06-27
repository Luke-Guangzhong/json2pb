/**
 * @file test_public_interface.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-26
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

void test_cvt_json_to_deprecated_field(void);
void test_cvt_json_to_already_setted_oneof_field(void);

void test_reject_null_root(void);
void test_reject_null_item(void);
void test_reject_null_msg(void);
void test_reject_null_field_name(void);

void test_reject_uninitialized_msg(void);
void test_reject_json_null(void);
void test_reject_unknown_field(void);
void test_reject_unknown_item(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

CU_TestInfo field_flag_tests[] = {
    {"Reject conversion of deprecated field",           test_cvt_json_to_deprecated_field          },
    {"Reject conversion of already setted oneof field", test_cvt_json_to_already_setted_oneof_field},
    CU_TEST_INFO_NULL,
};

CU_TestInfo illegal_argument_tests[] = {
    {"Reject NULL root",       test_reject_null_root      },
    {"Reject NULL item",       test_reject_null_item      },
    {"Reject NULL msg",        test_reject_null_msg       },
    {"Reject NULL field_name", test_reject_null_field_name},
    CU_TEST_INFO_NULL,
};

CU_TestInfo invalid_argument_tests[] = {
    {"Reject JSON null",         test_reject_json_null        },
    {"Reject unknown field",     test_reject_unknown_field    },
    {"Reject unknown item",      test_reject_unknown_item     },
    {"Reject uninitialized msg", test_reject_uninitialized_msg},
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Test Public Interface reject locked protobuf fields", NULL, NULL, setup_successful_conversion, teardown_successful_conversion, field_flag_tests      },
    {"Test Public Interface reject illegal arguments",      NULL, NULL, setup_successful_conversion, teardown_successful_conversion, illegal_argument_tests},
    {"Test Public Interface reject invalid arguments",      NULL, NULL, setup_redirect_stdout,       teardown_redirect_stdout,       invalid_argument_tests},
    CU_SUITE_INFO_NULL
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_to_deprecated_field(void)
{
    int32_t deprecated_value = 123456789;
    cJSON_AddNumberToObject(root, "deprecated_field", deprecated_value);

    j2p_expt_t e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "deprecated_field"), (ProtobufCMessage*)msg, "deprecated_field", NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_FIELD_IS_DEPRECATED);
    CU_ASSERT_EQUAL(msg->deprecated_field, 0);
}

void
test_cvt_json_to_already_setted_oneof_field(void)
{
    int32_t oneof_int32_value = 123456789;
    cJSON_AddNumberToObject(root, "oneof_int32", oneof_int32_value);

    j2p_expt_t                      e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "oneof_int32"), (ProtobufCMessage*)msg, "oneof_int32", NULL, NULL, J2P_FILE_PATH_STR);
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_int32");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_int32, oneof_int32_value);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);

    cJSON_AddNumberToObject(root, "oneof_sint32", 2345);
    e          = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "oneof_sint32"), (ProtobufCMessage*)msg, "oneof_sint32", NULL, NULL, J2P_FILE_PATH_STR);
    field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_sint32");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_ONEOF_ALREADY_SET);
    CU_ASSERT_NOT_EQUAL(msg->test_oneof_case, field_desc->id);
    CU_ASSERT_NOT_EQUAL(msg->oneof_sint32, 2345)
}

void
test_reject_null_root(void)
{
    cJSON_AddNumberToObject(root, "f_int32", 123456789);

    j2p_expt_t e = cvt_json_2_pb_field(NULL, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ARG);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_null_item(void)
{
    j2p_expt_t e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ARG);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_null_msg(void)
{
    cJSON_AddNumberToObject(root, "f_int32", 123456789);

    j2p_expt_t e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "f_int32"), NULL, "f_int32", NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ARG);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_null_field_name(void)
{
    cJSON_AddNumberToObject(root, "f_int32", 123456789);

    j2p_expt_t e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, NULL, NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ARG);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_uninitialized_msg(void)
{
    msg = (TestMessage*)calloc(1, sizeof(TestMessage));
    if (NULL == msg) {
        exit(EXIT_FAILURE);
    }
    root = cJSON_CreateObject();
    if (NULL == root) {
        exit(EXIT_FAILURE);
    }

    cJSON_AddNumberToObject(root, "f_int32", 12345);
    printf("root:\n%s\n", cJSON_Print(root));

    j2p_expt_t e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNINITIALIZED);
    CU_ASSERT_EQUAL(msg->f_int32, 0);

    free(msg);
    msg = NULL;
    cJSON_Delete(root);
    root = NULL;
}

void
test_reject_json_null(void)
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

    cJSON_AddNullToObject(root, "f_int32");

    j2p_expt_t e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NULL_VALUE);
    CU_ASSERT_EQUAL(msg->f_int32, 0);

    test_message__free_unpacked(msg, NULL);
    msg = NULL;
    cJSON_Delete(root);
    root = NULL;
}

void
test_reject_unknown_field(void)
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

    cJSON_AddNumberToObject(root, "unknown_field", 12345);

    j2p_expt_t e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "unknown_field"), (ProtobufCMessage*)msg, "unknown_field", NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_FIELD_NOT_FOUND);

    test_message__free_unpacked(msg, NULL);
    msg = NULL;
    cJSON_Delete(root);
    root = NULL;
}

void
test_reject_unknown_item(void)
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

    cJSON* another_root = cJSON_CreateObject();
    if (NULL == another_root) {
        exit(EXIT_FAILURE);
    };

    cJSON_AddNumberToObject(another_root, "f_int32", 12345);

    j2p_expt_t e = cvt_json_2_pb_field(root, cJSON_GetObjectItem(another_root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", NULL, NULL, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_JSON_POINTER_NOT_FOUND);
    CU_ASSERT_EQUAL(msg->f_int32, 0);

    test_message__free_unpacked(msg, NULL);
    msg = NULL;
    cJSON_Delete(root);
    root = NULL;
    cJSON_Delete(another_root);
    another_root = NULL;
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
