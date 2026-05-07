/**
 * @file test_cvt_to_enum.c
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

void test_cvt_json_number_to_single_enum(void);
void test_cvt_json_string_to_single_enum(void);
void test_cvt_json_number_to_oneof_enum(void);
void test_cvt_invalid_json_number_to_single_enum(void);
void test_cvt_invalid_json_string_to_single_enum(void);
void test_cvt_json_bool_to_single_enum(void);
void test_cvt_json_array_to_single_enum(void);
void test_cvt_json_object_to_single_enum(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char enum_field_name[]          = "f_enum";
const char repeated_enum_field_name[] = "f_repeated_enum";

CU_TestInfo test_enum_conversion[] = {
    {"Convert JSON number to Protobuf enum field",         test_cvt_json_number_to_single_enum},
    {"Convert JSON decimal string to Protobuf enum field", test_cvt_json_string_to_single_enum},
    {"Convert JSON number to Protobuf oneof enum field",   test_cvt_json_number_to_oneof_enum },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_enum_conversion_invalid[] = {
    {"Reject conversion of invalid JSON number to Protobuf enum field", test_cvt_invalid_json_number_to_single_enum},
    {"Reject conversion of invalid JSON string to Protobuf enum field", test_cvt_invalid_json_string_to_single_enum},
    {"Reject conversion of JSON bool to Protobuf enum field",           test_cvt_json_bool_to_single_enum          },
    {"Reject conversion of JSON array to Protobuf enum field",          test_cvt_json_array_to_single_enum         },
    {"Reject conversion of JSON object to Protobuf enum field",         test_cvt_json_object_to_single_enum        },
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf enum field",              init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_enum_conversion},
    {"Reject conversion of JSON to Protobuf enum field", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_enum_conversion_invalid                                                                                                                                              },
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_number_to_single_enum(void) {
    cJSON_AddNumberToObject(root, enum_field_name, ENUM__ENUM_2);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_2);
}

void
test_cvt_json_string_to_single_enum(void) {
    const char* value = "ENUM_3";

    cJSON_AddStringToObject(root, enum_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_2);
}

void
test_cvt_json_number_to_oneof_enum(void) {
    cJSON_AddNumberToObject(root, "oneof_enum", ENUM__ENUM_2);

    j2p_expt_t                      e          = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, "oneof_enum"), (ProtobufCMessage*)msg, "oneof_enum", NULL);
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_enum");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_enum, ENUM__ENUM_2);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

void
test_cvt_invalid_json_number_to_single_enum(void) {
    cJSON_AddNumberToObject(root, enum_field_name, 9);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ENUM_VALUE);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_0);
}

void
test_cvt_invalid_json_string_to_single_enum(void) {
    const char* value = "ENUM_9";

    cJSON_AddStringToObject(root, enum_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_ENUM_VALUE);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_0);
}

void
test_cvt_json_bool_to_single_enum(void) {
    cJSON_AddBoolToObject(root, enum_field_name, cJSON_True);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_0);
}

void
test_cvt_json_array_to_single_enum(void) {
    cJSON_AddArrayToObject(root, enum_field_name);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_0);
}

void
test_cvt_json_object_to_single_enum(void) {
    cJSON_AddObjectToObject(root, enum_field_name);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_0);
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
