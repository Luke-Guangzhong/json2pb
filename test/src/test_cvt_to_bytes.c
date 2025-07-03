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
#include <trower-base64/base64.h>
#include <unistd.h>

#include "json2pb.h"
#include "test.pb-c.h"
#include "utils.h"

/******************************************************************************/
/*                                Declarations                                */
/******************************************************************************/

void test_cvt_json_hex_string_to_single_bytes(void);
void test_cvt_json_base64_string_to_single_bytes(void);
void test_cvt_json_filepath_string_to_single_bytes(void);
void test_cvt_json_base64_string_to_oneof_bytes(void);

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

const char bytes_field_name[]          = "f_bytes";
const char repeated_bytes_field_name[] = "f_repeated_bytes";

CU_TestInfo test_float_conversion[] = {
    {"Convert JSON hex string to Protobuf bytes field",          test_cvt_json_hex_string_to_single_bytes     },
    {"Convert JSON base64 string to Protobuf bytes field",       test_cvt_json_base64_string_to_single_bytes  },
    {"Convert JSON filepath string to Protobuf bytes field",     test_cvt_json_filepath_string_to_single_bytes},
    {"Convert JSON base64 string to Protobuf oneof bytes field", test_cvt_json_base64_string_to_oneof_bytes   },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_float_overflow[] = {
#if 0
    {"Reject conversion of JSON number to Protobuf float on overflow",  test_cvt_json_number_overflow_float },
    {"Reject conversion of JSON number to Protobuf float on underflow", test_cvt_json_number_underflow_float},
    {"Reject conversion of JSON string to Protobuf float on overflow",  test_cvt_json_str_overflow_float    },
    {"Reject conversion of JSON string to Protobuf float on underflow", test_cvt_json_str_underflow_float   },
    {"Accept conversion of JSON number to Protobuf float at FLOAT_MAX", test_cvt_json_number_max_float      },
    {"Accept conversion of JSON number to Protobuf float at FLOAT_MIN", test_cvt_json_number_min_float      },
#endif
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_repeated_float_conversion[] = {
#if 0
    {"Convert JSON array to repeated Protobuf float field",              test_cvt_json_array_to_repeated_float              },
    {"Partial failure converting JSON array to repeated Protobuf float", test_cvt_json_array_to_repeated_float_partly_failed},
    {"All failure converting JSON array to repeated Protobuf float",     test_cvt_json_array_to_repeated_float_all_failed   },
    {"Convert Empty JSON array to repeated Protobuf float",              test_cvt_json_array_to_repeated_float_empty        },
#endif
    CU_TEST_INFO_NULL};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf double field", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_float_conversion},
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
test_cvt_json_hex_string_to_single_bytes(void)
{
    cJSON_AddStringToObject(root, bytes_field_name, "01 10 11 04");
    uint8_t      expected_bytes[] = {0x01, 0x10, 0x11, 0x04};
    const size_t expected_size    = sizeof(expected_bytes) / sizeof(expected_bytes[0]);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, J2P_FILE_HEX_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_bytes.len, expected_size);
    CU_ASSERT_EQUAL(memcmp(msg->f_bytes.data, expected_bytes, expected_size), 0);
}

void
test_cvt_json_base64_string_to_single_bytes(void)
{
    uint8_t      expected_bytes[] = {0x01, 0x10, 0x11, 0x04};
    const size_t expected_size    = sizeof(expected_bytes) / sizeof(expected_bytes[0]);

    size_t out_len    = 0;
    char*  base64_str = b64_encode_with_alloc(expected_bytes, expected_size, &out_len);

    cJSON_AddStringToObject(root, bytes_field_name, base64_str);
    free(base64_str);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, J2P_FILE_BASE64_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_bytes.len, expected_size);
    CU_ASSERT_EQUAL(memcmp(msg->f_bytes.data, expected_bytes, expected_size), 0);
}

void
test_cvt_json_filepath_string_to_single_bytes(void)
{
    uint8_t      expected_bytes[] = {0x01, 0x10, 0x11, 0x04};
    const size_t expected_size    = sizeof(expected_bytes) / sizeof(expected_bytes[0]);

    FILE* fp = fopen("test_cvt_json_filepath_string_to_single_bytes.bin", "wb");
    if (NULL == fp) {
        CU_FAIL("Failed to open test file for writing");
    }
    fwrite(expected_bytes, expected_size, 1, fp);
    fclose(fp);

    cJSON_AddStringToObject(root, bytes_field_name, "test_cvt_json_filepath_string_to_single_bytes.bin");

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_bytes.len, expected_size);
    CU_ASSERT_EQUAL(memcmp(msg->f_bytes.data, expected_bytes, expected_size), 0);

    unlink("test_cvt_json_filepath_string_to_single_bytes.bin");
}

void
test_cvt_json_base64_string_to_oneof_bytes(void)
{
    uint8_t      expected_bytes[] = {0x01, 0x10, 0x11, 0x04};
    const size_t expected_size    = sizeof(expected_bytes) / sizeof(expected_bytes[0]);

    size_t out_len    = 0;
    char*  base64_str = b64_encode_with_alloc(expected_bytes, expected_size, &out_len);

    cJSON_AddStringToObject(root, "oneof_bytes", base64_str);
    free(base64_str);

    j2p_expt_t                      e          = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, "oneof_bytes"), (ProtobufCMessage*)msg, "oneof_bytes", J2P_FILE_BASE64_STR);
    const ProtobufCFieldDescriptor* field_desc = protobuf_c_message_descriptor_get_field_by_name(msg->base.descriptor, "oneof_bytes");
    CU_ASSERT_PTR_NOT_NULL_FATAL(field_desc);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->oneof_bytes.len, expected_size);
    CU_ASSERT_EQUAL(memcmp(msg->oneof_bytes.data, expected_bytes, expected_size), 0);
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
