/**
 * @file test_cvt_to_string.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-07-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "json2pb.h"
#include "test.pb-c.h"
#include "utils.h"
#include <stdbool.h>
#include <unistd.h>

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

void test_cvt_json_string_to_single_string(void);
void test_cvt_json_string_to_oneof_string(void);

void test_cvt_json_object_to_single_string(void);
void test_cvt_json_array_to_single_string(void);
void test_cvt_json_number_to_single_string(void);
void test_cvt_json_bool_to_single_string(void);

void test_cvt_json_array_to_repeated_string(void);
void test_cvt_json_array_to_repeated_string_partly_failed(void);
void test_cvt_json_array_to_repeated_string_all_failed(void);
void test_cvt_json_array_to_repeated_string_empty(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char string_field_name[]          = "f_string";
const char repeated_string_field_name[] = "f_repeated_string";

CU_TestInfo test_string_conversion[] = {
    {"Convert JSON string to Protobuf string field",       test_cvt_json_string_to_single_string},
    {"Convert JSON string to Protobuf oneof string field", test_cvt_json_string_to_oneof_string },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_reject_invalid_conversion[] = {
    {"Reject conversion of JSON object to protobuf string", test_cvt_json_object_to_single_string},
    {"Reject conversion of JSON array to protobuf string",  test_cvt_json_array_to_single_string },
    {"Reject conversion of JSON number to protobuf string", test_cvt_json_number_to_single_string},
    {"Reject conversion of JSON bool to protobuf string",   test_cvt_json_bool_to_single_string  },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_string_conversion[] = {
    {"Convert JSON array to Protobuf repeated string feild",                    test_cvt_json_array_to_repeated_string              },
    {"Convert JSON array to Protobuf repeated string feild partily failed",     test_cvt_json_array_to_repeated_string_partly_failed},
    {"Convert JSON array to Protobuf repeated string field all failed",         test_cvt_json_array_to_repeated_string_all_failed   },
    {"Reject conversion of empty JSON array to Protobuf repeated string field", test_cvt_json_array_to_repeated_string_empty        },
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf string field",                init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_string_conversion},
    {"Convert unacceptable JSON to Protobuf string field",   init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_reject_invalid_conversion                                                                                                                                                  },
    {"Convert JSON array to Protobuf repeated string field", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_repeated_string_conversion                                                                                                                                                 },
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_string_to_single_string(void) {
    cJSON_AddStringToObject(root, string_field_name, "Test String");

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, string_field_name), (ProtobufCMessage*)msg, string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_STRING_EQUAL(msg->f_string, "Test String");
}

void
test_cvt_json_string_to_oneof_string(void) {
    cJSON_AddStringToObject(root, "oneof_string", "Test String");

    j2p_expt_t                      e          = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, "oneof_string"), (ProtobufCMessage*)msg, "oneof_string");
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_string");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_STRING_EQUAL(msg->oneof_string, "Test String");
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

void
test_cvt_json_object_to_single_string(void) {
    cJSON_AddObjectToObject(root, string_field_name);

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, string_field_name), (ProtobufCMessage*)msg, string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_string, protobuf_c_empty_string);
}

void
test_cvt_json_array_to_single_string(void) {
    cJSON_AddArrayToObject(root, string_field_name);

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, string_field_name), (ProtobufCMessage*)msg, string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_string, protobuf_c_empty_string);
}

void
test_cvt_json_number_to_single_string(void) {
    cJSON_AddNumberToObject(root, string_field_name, 123);

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, string_field_name), (ProtobufCMessage*)msg, string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_string, protobuf_c_empty_string);
}

void
test_cvt_json_bool_to_single_string(void) {
    cJSON_AddBoolToObject(root, string_field_name, cJSON_True);

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, string_field_name), (ProtobufCMessage*)msg, string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_string, protobuf_c_empty_string);
}

void
test_cvt_json_array_to_repeated_string(void) {
    const char*  value          = "[\"test string 1\", \"test string 2\"]";
    const char*  expect_array[] = {"test string 1", "test string 2"};
    const size_t expect_length  = sizeof(expect_array) / sizeof(char*);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_string_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, repeated_string_field_name), (ProtobufCMessage*)msg, repeated_string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_string, expect_length);
    for (size_t i = 0; i < msg->n_f_repeated_string; i++) {
        CU_ASSERT_STRING_EQUAL(msg->f_repeated_string[i], expect_array[i]);
    }
}

void
test_cvt_json_array_to_repeated_string_partly_failed(void) {
    const char*  value          = "[\"test string 1\", \"test string 2\",1234]";
    const char*  expect_array[] = {"test string 1", "test string 2"};
    const size_t expect_length  = sizeof(expect_array) / sizeof(char*);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_string_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, repeated_string_field_name), (ProtobufCMessage*)msg, repeated_string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_PARTIAL_FAIL);
    CU_ASSERT_EQUAL(msg->n_f_repeated_string, expect_length);
    for (size_t i = 0; i < msg->n_f_repeated_string; i++) {
        CU_ASSERT_STRING_EQUAL(msg->f_repeated_string[i], expect_array[i]);
    }
}

void
test_cvt_json_array_to_repeated_string_all_failed(void) {
    const char* value = "[1234]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_string_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, repeated_string_field_name), (ProtobufCMessage*)msg, repeated_string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NO_VALID_FOUND);
    CU_ASSERT_EQUAL(msg->n_f_repeated_string, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_string);
}

void
test_cvt_json_array_to_repeated_string_empty(void) {
    cJSON_AddArrayToObject(root, repeated_string_field_name);

    j2p_expt_t e = cvt_json_2_pb_string(root, cJSON_GetObjectItem(root, repeated_string_field_name), (ProtobufCMessage*)msg, repeated_string_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_EMPTY_ARRAY);
    CU_ASSERT_EQUAL(msg->n_f_repeated_string, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_string);
}

/******************************************************************************/
/*                                 Main Code                                  */
/******************************************************************************/

int
main(int argc, char const* argv[]) {
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
