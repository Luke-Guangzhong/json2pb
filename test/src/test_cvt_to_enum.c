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

#include <stdbool.h>
#include <unistd.h>

#include "json2pb.h"
#include "test.pb-c.h"
#include "utils.h"

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

void test_cvt_json_number_to_single_enum(void);
void test_cvt_json_string_to_single_enum(void);
void test_cvt_json_number_to_oneof_enum(void);

#if 0

void test_cvt_json_number_overflow_float(void);
void test_cvt_json_number_underflow_float(void);
void test_cvt_json_str_overflow_float(void);
void test_cvt_json_str_underflow_float(void);
void test_cvt_json_number_max_float(void);
void test_cvt_json_number_min_float(void);
void test_cvt_json_str_max_float(void);
void test_cvt_json_str_min_float(void);

void test_cvt_json_array_to_repeated_float(void);
void test_cvt_json_array_to_repeated_float_partly_failed(void);
void test_cvt_json_array_to_repeated_float_all_failed(void);
void test_cvt_json_array_to_repeated_float_empty(void);

#endif

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

#if 0

CU_TestInfo test_float_overflow[] = {
    {"Reject conversion of JSON number to Protobuf float on overflow",  test_cvt_json_number_overflow_float },
    {"Reject conversion of JSON number to Protobuf float on underflow", test_cvt_json_number_underflow_float},
    {"Reject conversion of JSON string to Protobuf float on overflow",  test_cvt_json_str_overflow_float    },
    {"Reject conversion of JSON string to Protobuf float on underflow", test_cvt_json_str_underflow_float   },
    {"Accept conversion of JSON number to Protobuf float at FLOAT_MAX", test_cvt_json_number_max_float      },
    {"Accept conversion of JSON number to Protobuf float at FLOAT_MIN", test_cvt_json_number_min_float      },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_float_conversion[] = {
    {"Convert JSON array to repeated Protobuf float field",              test_cvt_json_array_to_repeated_float              },
    {"Partial failure converting JSON array to repeated Protobuf float", test_cvt_json_array_to_repeated_float_partly_failed},
    {"All failure converting JSON array to repeated Protobuf float",     test_cvt_json_array_to_repeated_float_all_failed   },
    {"Convert Empty JSON array to repeated Protobuf float",              test_cvt_json_array_to_repeated_float_empty        },
    CU_TEST_INFO_NULL
};

#endif

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf enum field", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_enum_conversion},
#if 0
    {"Convert JSON to Protobuf float with overflow handling", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_float_overflow                                                                                                                                                             },
    {"Convert JSON array to repeated Protobuf float field",   init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_repeated_float_conversion                                                                                                                                                  },
#endif
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_number_to_single_enum(void)
{
    cJSON_AddNumberToObject(root, enum_field_name, ENUM__ENUM_2);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_2);
    printf("msg->f_enum = %d\n", msg->f_enum);
}

void
test_cvt_json_string_to_single_enum(void)
{
    const char* value = "ENUM_3";

    cJSON_AddStringToObject(root, enum_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, enum_field_name), (ProtobufCMessage*)msg, enum_field_name, NULL);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_enum, ENUM__ENUM_2);
}

void
test_cvt_json_number_to_oneof_enum(void)
{
    cJSON_AddNumberToObject(root, "oneof_enum", ENUM__ENUM_2);

    j2p_expt_t                      e          = cvt_json_2_pb_enum(root, cJSON_GetObjectItem(root, "oneof_enum"), (ProtobufCMessage*)msg, "oneof_enum", NULL);
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_enum");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_enum, ENUM__ENUM_2);
    CU_ASSERT_EQUAL(msg->test_oneof_case, field_desc->id);
}

#if 0

void
test_cvt_json_number_overflow_float(void)
{
    double value = 1e40;

    cJSON_AddNumberToObject(root, float_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, float_field_name), (ProtobufCMessage*)msg, float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_float, 0);
}

void
test_cvt_json_number_underflow_float(void)
{
    double value = 1e-50;

    cJSON_AddNumberToObject(root, float_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, float_field_name), (ProtobufCMessage*)msg, float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_float, 0);
}

void
test_cvt_json_str_overflow_float(void)
{
    const char* value = "3.5e38";

    cJSON_AddStringToObject(root, float_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, float_field_name), (ProtobufCMessage*)msg, float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_float, 0);
}

void
test_cvt_json_str_underflow_float(void)
{
    const char* value = "1.0e-46";

    cJSON_AddStringToObject(root, float_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, float_field_name), (ProtobufCMessage*)msg, float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_VALUE_OVERFLOW);
    CU_ASSERT_EQUAL(msg->f_float, 0);
}

void
test_cvt_json_number_max_float(void)
{
    double value = (double)FLT_MAX;

    cJSON_AddNumberToObject(root, float_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, float_field_name), (ProtobufCMessage*)msg, float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_float, value);
}

void
test_cvt_json_number_min_float(void)
{
    double value = (double)FLT_TRUE_MIN;

    cJSON_AddNumberToObject(root, float_field_name, value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, float_field_name), (ProtobufCMessage*)msg, float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_float, value);
}

void
test_cvt_json_array_to_repeated_float(void)
{
    const char*  value          = "[123.0, \"0.5\"]";
    const float  expect_array[] = {123.0, 0.5};
    const size_t expect_length  = sizeof(expect_array) / sizeof(float);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_float_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_float_field_name), (ProtobufCMessage*)msg, repeated_float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->n_f_repeated_float, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_float, expect_array, expect_length * sizeof(float)), 0);
}

void
test_cvt_json_array_to_repeated_float_partly_failed(void)
{
    const char*  value          = "[123.0, \"0.5\",\"invalid number string\"]";
    const float  expect_array[] = {123.0, 0.5};
    const size_t expect_length  = sizeof(expect_array) / sizeof(float);

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_float_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_float_field_name), (ProtobufCMessage*)msg, repeated_float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_PARTIAL_FAIL);
    CU_ASSERT_EQUAL(msg->n_f_repeated_float, expect_length);
    CU_ASSERT_EQUAL(memcmp(msg->f_repeated_float, expect_array, expect_length * sizeof(float)), 0);
}

void
test_cvt_json_array_to_repeated_float_all_failed(void)
{
    const char* value = "[\"invalid number string\"]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_float_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_float_field_name), (ProtobufCMessage*)msg, repeated_float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_NO_VALID_FOUND);
    CU_ASSERT_EQUAL(msg->n_f_repeated_float, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_float);
}

void
test_cvt_json_array_to_repeated_float_empty(void)
{
    const char* value = "[]";

    cJSON* array_value = cJSON_Parse(value);
    cJSON_AddItemToObject(root, repeated_float_field_name, array_value);

    j2p_expt_t e = cvt_json_2_pb_number(root, cJSON_GetObjectItem(root, repeated_float_field_name), (ProtobufCMessage*)msg, repeated_float_field_name);
    CU_ASSERT_EQUAL(e, J2P_EXPT_EMPTY_ARRAY);
    CU_ASSERT_EQUAL(msg->n_f_repeated_float, 0);
    CU_ASSERT_PTR_NULL(msg->f_repeated_float);
}

#endif

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
