/**
 * @file test_cvt_to_message.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-07-05
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

void test_cvt_json_object_to_single_message(void);
void test_cvt_json_object_to_oneof_message(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char message_field_name[]          = "f_message_in_test_message";
const char repeated_message_field_name[] = "f_repeated_message";

CU_TestInfo test_message_conversion[] = {
    {"Convert JSON object to Protobuf message field",       test_cvt_json_object_to_single_message},
    {"Convert JSON object to Protobuf oneof message field", test_cvt_json_object_to_oneof_message },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_message_overflow[] = {
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_message_conversion[] = {CU_TEST_INFO_NULL};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf message field", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_message_conversion},
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_object_to_single_message(void)
{
    const char* json_str = "{\"f_int32\":123,\"f_sint32\":\"0x4A0\",\"f_sfixed32\":\"0123\","
                           "\"f_repeated_int32\":[1234567890,\"1234567890\",\"0x4a0\",\"0110\",123.45],"
                           "\"f_repeated_sint32\":[1234567890,\"1234567890\",\"0x4a0\",\"0110\",123.45],"
                           "\"f_repeated_sfixed32\":[1234567890,\"1234567890\",\"0x4a0\",\"0110\",123.45]}";
    cJSON*      json_obj = cJSON_Parse(json_str);
    CU_ASSERT_PTR_NOT_NULL_FATAL(json_obj);

    cJSON_AddItemToObject(root, "f_message_in_test_message", json_obj);

    j2p_expt_t e = cvt_json_2_pb_message(root, cJSON_GetObjectItem(root, "f_message_in_test_message"), (ProtobufCMessage*)msg, "f_message_in_test_message", NULL);

    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(msg->f_message_in_test_message);
    CU_ASSERT_EQUAL(msg->f_message_in_test_message->f_int32, 123);
    CU_ASSERT_EQUAL(msg->f_message_in_test_message->f_sint32, 0x4A0);
    CU_ASSERT_EQUAL(msg->f_message_in_test_message->f_sfixed32, 0123);
    CU_ASSERT_EQUAL(msg->f_message_in_test_message->n_f_repeated_int32, 5);
    CU_ASSERT_EQUAL(msg->f_message_in_test_message->n_f_repeated_sint32, 5);
    CU_ASSERT_EQUAL(msg->f_message_in_test_message->n_f_repeated_sfixed32, 5);
}

void
test_cvt_json_object_to_oneof_message(void)
{
    const char* json_str = "{\"f_int32\":456,\"f_sint32\":789}";
    cJSON*      json_obj = cJSON_Parse(json_str);
    CU_ASSERT_PTR_NOT_NULL_FATAL(json_obj);

    cJSON_AddItemToObject(root, "oneof_message", json_obj);

    j2p_expt_t                      e          = cvt_json_2_pb_message(root, cJSON_GetObjectItem(root, "oneof_message"), (ProtobufCMessage*)msg, "oneof_message", NULL);
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_message");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(msg->oneof_message);
    CU_ASSERT_EQUAL(msg->oneof_message->f_int32, 456);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
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
