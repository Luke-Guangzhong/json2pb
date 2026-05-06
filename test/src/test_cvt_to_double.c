/**
 * @file test_cvt_to_double.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-07-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <float.h>
#include <unistd.h>

#include "json2pb.h"
#include "test.pb-c.h"
#include "utils.h"

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

void test_cvt_json_number_to_single_double(void);
void test_cvt_json_decimal_string_to_single_double(void);
void test_cvt_json_number_to_oneof_double(void);

void test_cvt_json_str_overflow_double(void);
void test_cvt_json_nan_to_double(void);
void test_cvt_json_inf_to_double(void);
void test_cvt_json_str_nan_to_double(void);
void test_cvt_json_str_inf_to_double(void);
void test_cvt_json_bool_to_double(void);
void test_cvt_json_array_to_double(void);
void test_cvt_json_object_to_double(void);

void test_cvt_json_array_to_repeated_double(void);
void test_cvt_json_array_to_repeated_double_partly_failed(void);
void test_cvt_json_array_to_repeated_double_all_failed(void);
void test_cvt_json_array_to_repeated_double_empty(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char double_field_name[]          = "f_double";
const char repeated_double_field_name[] = "f_repeated_double";

CU_TestInfo test_double_conversion[] = {
    {"Convert JSON number to Protobuf double field",         test_cvt_json_number_to_single_double        },
    {"Convert JSON decimal string to Protobuf double field", test_cvt_json_decimal_string_to_single_double},
    {"Convert JSON number to Protobuf oneof double field",   test_cvt_json_number_to_oneof_double         },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_double_overflow[] = {
    {"Reject conversion of JSON string to Protobuf double on overflow",                test_cvt_json_str_overflow_double},
    {"Reject conversion of invalid cjson number (inf) to protobuf double on overflow", test_cvt_json_inf_to_double      },
    {"Reject conversion of invalid cjson number (nan) to protobuf double on overflow", test_cvt_json_nan_to_double      },
    {"Reject conversion of invalid cjson string (inf) to protobuf double on overflow", test_cvt_json_str_nan_to_double  },
    {"Reject conversion of invalid cjson string (nan) to protobuf double on overflow", test_cvt_json_str_inf_to_double  },
    {"Reject conversion of JSON bool to Protobuf double",                              test_cvt_json_bool_to_double     },
    {"Reject conversion of JSON array to Protobuf double",                             test_cvt_json_array_to_double    },
    {"Reject conversion of JSON object to Protobuf double",                            test_cvt_json_object_to_double   },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_double_conversion[] = {
    {"Convert JSON array to repeated Protobuf double field",              test_cvt_json_array_to_repeated_double              },
    {"Partial failure converting JSON array to repeated Protobuf double", test_cvt_json_array_to_repeated_double_partly_failed},
    {"All failure converting JSON array to repeated Protobuf double",     test_cvt_json_array_to_repeated_double_all_failed   },
    {"Convert Empty JSON array to repeated Protobuf double",              test_cvt_json_array_to_repeated_double_empty        },
    CU_TEST_INFO_NULL
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf double field",                 init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_double_conversion},
    {"Convert JSON to Protobuf float with overflow handling", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_double_overflow                                                                                                                                                             },
    {"Convert JSON array to repeated Protobuf float field",   init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_repeated_double_conversion                                                                                                                                                  },
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_number_to_single_double(void)
{
    double value = 0.5f;

    cJSON_AddNumberToObject(root, double_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_double, value);
}

void
test_cvt_json_decimal_string_to_single_double(void)
{
    const char* value = "0.5";

    cJSON_AddStringToObject(root, double_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_double, 0.5f);
}

void
test_cvt_json_number_to_oneof_double(void)
{
    float oneof_double_value = 0.5f;
    cJSON_AddNumberToObject(root, "oneof_double", oneof_double_value);

    j2p_expt_t                      e          = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, "oneof_double"), (ProtobufCMessage*)msg, "oneof_double");
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_double");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_double, oneof_double_value);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

void
test_cvt_json_str_overflow_double(void)
{
    const char* value = "9.999999e999";

    cJSON_AddStringToObject(root, double_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_double, 0);
}

void
test_cvt_json_nan_to_double(void)
{
    cJSON_AddNumberToObject(root, double_field_name, NAN);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_double, 0);
}

void
test_cvt_json_inf_to_double(void)
{
    cJSON_AddNumberToObject(root, double_field_name, INFINITY);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_double, 0);
}

void
test_cvt_json_str_nan_to_double(void)
{
    cJSON_AddStringToObject(root, double_field_name, "nan");

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_double, 0);
}

void
test_cvt_json_str_inf_to_double(void)
{
    cJSON_AddStringToObject(root, double_field_name, "inf");

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_double, 0);
}

void
test_cvt_json_bool_to_double(void)
{
    cJSON_AddBoolToObject(root, double_field_name, cJSON_True);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_double, 0);
}

void
test_cvt_json_array_to_double(void)
{
    cJSON_AddArrayToObject(root, double_field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_double, 0);
}

void
test_cvt_json_object_to_double(void)
{
    cJSON_AddObjectToObject(root, double_field_name);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, double_field_name), (ProtobufCMessage*)msg, double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_double, 0);
}

void
test_cvt_json_array_to_repeated_double(void)
{
    const char*  value          = "[123.0, \"0.5\"]";
    const double expect_array[] = {123.0, 0.5};
    const size_t expect_length  = sizeof(expect_array) / sizeof(double);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_double_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_double_field_name), (ProtobufCMessage*)msg, repeated_double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_double, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_double, expect_array, expect_length * sizeof(float)), 0);
}

void
test_cvt_json_array_to_repeated_double_partly_failed(void)
{
    const char*  value          = "[123.0, \"0.5\",\"invalid number string\"]";
    const double expect_array[] = {123.0, 0.5};
    const size_t expect_length  = sizeof(expect_array) / sizeof(double);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_double_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_double_field_name), (ProtobufCMessage*)msg, repeated_double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_PARTIAL_FAIL);
    CU_ASSERT_EQUAL(msg->n_f_repeated_double, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_double, expect_array, expect_length * sizeof(float)), 0);
}

void
test_cvt_json_array_to_repeated_double_all_failed(void)
{
    const char* value = "[\"invalid number string\"]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_double_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_double_field_name), (ProtobufCMessage*)msg, repeated_double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NO_VALID_FOUND);
    CU_ASSERT_EQUAL(msg->n_f_repeated_double, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_double);
}

void
test_cvt_json_array_to_repeated_double_empty(void)
{
    const char* value = "[]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_double_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_double_field_name), (ProtobufCMessage*)msg, repeated_double_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_EMPTY_ARRAY);
    CU_ASSERT_EQUAL(msg->n_f_repeated_double, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_double);
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
