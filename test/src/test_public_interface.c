/**
 * @file test_public_interface.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <unistd.h>

#include "json2pb.h"
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
void test_cvt_json_bool_to_repeated_field(void);
void test_cvt_json_number_to_repeated_field(void);
void test_cvt_json_string_to_repeated_field(void);
void test_cvt_json_object_to_repeated_field(void);

void test_cvt_json_bool_to_single_number(void);
void test_cvt_json_array_to_single_number(void);
void test_cvt_json_object_to_single_number(void);
void test_cvt_invalid_json_str_to_single_number(void);

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
    {"Reject JSON null",                              test_reject_json_null                 },
    {"Reject unknown field",                          test_reject_unknown_field             },
    {"Reject unknown item",                           test_reject_unknown_item              },
    {"Reject uninitialized msg",                      test_reject_uninitialized_msg         },
    {"Reject conversion of bool to repeated field",   test_cvt_json_bool_to_repeated_field  },
    {"Reject conversion of number to repeated field", test_cvt_json_number_to_repeated_field},
    {"Reject conversion of string to repeated field", test_cvt_json_string_to_repeated_field},
    {"Reject conversion of object to repeated field", test_cvt_json_object_to_repeated_field},
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Test Public Interface reject locked protobuf fields", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, field_flag_tests      },
    {"Test Public Interface reject illegal arguments",      init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, illegal_argument_tests},
    {"Test Public Interface reject invalid arguments",      init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, invalid_argument_tests},
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

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "deprecated_field"), (ProtobufCMessage*)msg, "deprecated_field", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_FIELD_IS_DEPRECATED);
    CU_ASSERT_EQUAL(msg->deprecated_field, 0);
}

void
test_cvt_json_to_already_setted_oneof_field(void)
{
    int32_t oneof_int32_value = 123456789;
    cJSON_AddNumberToObject(root, "oneof_int32", oneof_int32_value);

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "oneof_int32"), (ProtobufCMessage*)msg, "oneof_int32", (j2p_add_cvt){.bool_cvt = NULL});
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_int32");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_int32, oneof_int32_value);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);

    cJSON_AddNumberToObject(root, "oneof_sint32", 2345);
    e          = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "oneof_sint32"), (ProtobufCMessage*)msg, "oneof_sint32", (j2p_add_cvt){.bool_cvt = NULL});
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

    j2p_expt_t e = cvt_json_2_pb_field_v2(NULL, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ARG);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_null_item(void)
{
    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ARG);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_null_msg(void)
{
    cJSON_AddNumberToObject(root, "f_int32", 123456789);

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_int32"), NULL, "f_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ARG);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_null_field_name(void)
{
    cJSON_AddNumberToObject(root, "f_int32", 123456789);

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, NULL, (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ARG);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_uninitialized_msg(void)
{
    test_message__free_unpacked(msg, NULL);
    msg = (TestMessage*)calloc(1, sizeof(TestMessage));

    cJSON_AddNumberToObject(root, "f_int32", 12345);

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNINITIALIZED);
    CU_ASSERT_EQUAL(msg->f_int32, 0);

    free(msg);
    msg = NULL;
}

void
test_cvt_json_bool_to_repeated_field(void)
{
    cJSON_AddBoolToObject(root, "f_repeated_int32", true);

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_repeated_int32"), (ProtobufCMessage*)msg, "f_repeated_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
}

void
test_cvt_json_number_to_repeated_field(void)
{
    cJSON_AddNumberToObject(root, "f_repeated_int32", 12345);

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_repeated_int32"), (ProtobufCMessage*)msg, "f_repeated_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
}

void
test_cvt_json_string_to_repeated_field(void)
{
    cJSON_AddStringToObject(root, "f_repeated_int32", "test string");

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_repeated_int32"), (ProtobufCMessage*)msg, "f_repeated_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
}

void
test_cvt_json_object_to_repeated_field(void)
{
    cJSON_AddObjectToObject(root, "f_repeated_int32");

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_repeated_int32"), (ProtobufCMessage*)msg, "f_repeated_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->n_f_repeated_int32, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_int32);
}

void
test_reject_json_null(void)
{
    cJSON_AddNullToObject(root, "f_int32");

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_int32, 0);
}

void
test_reject_unknown_field(void)
{
    cJSON_AddNumberToObject(root, "unknown_field", 12345);

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(root, "unknown_field"), (ProtobufCMessage*)msg, "unknown_field", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_FIELD_NOT_FOUND);
}

void
test_reject_unknown_item(void)
{
    cJSON* another_root = cJSON_CreateObject();

    cJSON_AddNumberToObject(another_root, "f_int32", 12345);

    j2p_expt_t e = cvt_json_2_pb_field_v2(root, cJSON_GetObjectItem(another_root, "f_int32"), (ProtobufCMessage*)msg, "f_int32", (j2p_add_cvt){.bool_cvt = NULL});
    CU_ASSERT_EQUAL(e, J2P_EXPT_JSON_POINTER_NOT_FOUND);
    CU_ASSERT_EQUAL(msg->f_int32, 0);

    cJSON_Delete(another_root);
    another_root = NULL;
}

/******************************************************************************/
/*                                 Main Code                                  */
/******************************************************************************/

int
main(int argc, char const* argv[])
{
    char current_source_file_path[PATH_MAX] = __FILE__;
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
