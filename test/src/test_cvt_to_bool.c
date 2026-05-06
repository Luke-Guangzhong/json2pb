/**
 * @file test_cvt_to_bool.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-07-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdbool.h>
#include <unistd.h>

#include "json2pb.h"
#include "test.pb-c.h"
#include "utils.h"

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

void test_cvt_json_bool_to_single_bool(void);
void test_cvt_json_string_to_single_bool(void);
void test_cvt_json_bool_to_oneof_bool(void);
void test_cvt_json_number_to_single_bool(void);
void test_cvt_json_array_to_bool(void);
void test_cvt_json_object_to_bool(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char bool_field_name[]          = "f_bool";
const char repeated_bool_field_name[] = "f_repeated_bool";

CU_TestInfo test_bool_conversion[] = {
    {"Convert JSON bool to Protobuf bool field",          test_cvt_json_bool_to_single_bool  },
    {"Convert JSON string to Protobuf bool field",        test_cvt_json_string_to_single_bool},
    {"Convert JSON bool to Protobuf oneof bool field",    test_cvt_json_bool_to_oneof_bool   },
    {"Convert JSON number to Protobuf bool field",        test_cvt_json_number_to_single_bool},
    {"Reject Conversion of JSON array to Protobuf bool",  test_cvt_json_array_to_bool        },
    {"Reject Conversion of JSON object to Protobuf bool", test_cvt_json_object_to_bool       },
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf double field", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_bool_conversion},
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_bool_to_single_bool(void)
{
    cJSON_AddTrueToObject(root, bool_field_name);

    j2p_expt_t e = cvt_json_2_pb_bool(root, cJSON_GetObjectItem(root, bool_field_name), (ProtobufCMessage*)msg, bool_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_bool, true);
}

void
test_cvt_json_string_to_single_bool(void)
{
    const char* value = "true";

    cJSON_AddStringToObject(root, bool_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_bool(root, cJSON_GetObjectItem(root, bool_field_name), (ProtobufCMessage*)msg, bool_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_bool, true);
}

void
test_cvt_json_bool_to_oneof_bool(void)
{
    cJSON_AddTrueToObject(root, "oneof_bool");

    j2p_expt_t                      e          = cvt_json_2_pb_bool(root, cJSON_GetObjectItem(root, "oneof_bool"), (ProtobufCMessage*)msg, "oneof_bool", NULL);
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_bool");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_bool, true);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

void
test_cvt_json_number_to_single_bool(void)
{
    cJSON_AddNumberToObject(root, bool_field_name, 1);

    j2p_expt_t e = cvt_json_2_pb_bool(root, cJSON_GetObjectItem(root, bool_field_name), (ProtobufCMessage*)msg, bool_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_bool, true);

    cJSON_DeleteItemFromObject(root, bool_field_name);

    cJSON_AddNumberToObject(root, bool_field_name, 0);

    e = cvt_json_2_pb_bool(root, cJSON_GetObjectItem(root, bool_field_name), (ProtobufCMessage*)msg, bool_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_bool, false);
}

void
test_cvt_json_array_to_bool(void)
{
    cJSON_AddArrayToObject(root, bool_field_name);

    j2p_expt_t e = cvt_json_2_pb_bool(root, cJSON_GetObjectItem(root, bool_field_name), (ProtobufCMessage*)msg, bool_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_bool, false);
}

void
test_cvt_json_object_to_bool(void)
{
    cJSON_AddObjectToObject(root, bool_field_name);

    j2p_expt_t e = cvt_json_2_pb_bool(root, cJSON_GetObjectItem(root, bool_field_name), (ProtobufCMessage*)msg, bool_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_bool, false);
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
