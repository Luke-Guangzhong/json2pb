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
#include <float.h>
#include <trower-base64/base64.h>
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

const char bytes_field_name[]          = "f_bytes";
const char repeated_bytes_field_name[] = "f_repeated_bytes";

CU_TestInfo test_float_conversion[] = {
    {"Convert JSON object to Protobuf message field",            test_cvt_json_object_to_single_message},
    {"Convert JSON base64 string to Protobuf oneof bytes field", test_cvt_json_object_to_oneof_message },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_float_overflow[] = {
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_float_conversion[] = {CU_TEST_INFO_NULL};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf double field", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_float_conversion},
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_object_to_single_message(void)
{
    const char* json_str =
        "{\"f_int32\":123,\"f_sint32\":\"0x4A0\",\"f_sfixed32\":\"0123\",\"f_repeated_int32\":[1234567890,\"1234567890\",\"0x4a0\",\"0110\",123.45],"
        "\"f_repeated_sint32\":[1234567890,\"1234567890\",\"0x4a0\",\"0110\",123.45],\"f_repeated_sfixed32\":[1234567890,\"1234567890\",\"0x4a0\",\"0110\",123.45],\"deprecated_"
        "field\":\"test\",\"f_int64\":\"\",\"f_sint64\":\"\",\"f_sfixed64\":\"\",\"f_repeated_int64\":\"\",\"f_repeated_sint64\":\"\",\"f_repeated_sfixed64\":\"\",\"f_double\":"
        "\"\",\"f_repeated_double\":\"\",\"f_float\":\"\",\"f_repeated_float\":\"\",\"f_uint32\":\"\",\"f_fixed32\":\"\",\"f_repeated_uint32\":\"\",\"f_repeated_fixed32\":\"\","
        "\"f_uint64\":\"\",\"f_fixed64\":\"\",\"f_repeated_uint64\":\"\",\"f_repeated_fixed64\":\"\",\"f_bool\":\"\",\"f_repeated_bool\":\"\",\"f_string\":\"\",\"f_repeated_"
        "string\":\"\",\"f_bytes\":\"\",\"f_repeated_bytes\":\"\",\"f_enum\":\"\",\"f_repeated_enum\":\"\",\"f_message_in_test_message\":\"\"}";
    cJSON* json_obj = cJSON_Parse(json_str);
    CU_ASSERT_PTR_NOT_NULL_FATAL(json_obj);

    cJSON_AddItemToObject(root, "f_message_in_test_message", json_obj);

    j2p_expt_t e =
        cvt_json_2_pb_field(root, cJSON_GetObjectItem(root, "f_message_in_test_message"), (ProtobufCMessage*)msg, "f_message_in_test_message", NULL, NULL, J2P_FILE_PATH_STR, NULL);

    CU_ASSERT_EQUAL(msg->f_message_in_test_message->f_int32, 123);
}

void
test_cvt_json_object_to_oneof_message(void)
{
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
