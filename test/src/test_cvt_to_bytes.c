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

void test_cvt_invalid_hex_string_to_single_bytes(void);
void test_cvt_invalid_base64_string_to_single_bytes(void);
void test_cvt_invalid_filepath_string_to_single_bytes(void);
void test_cvt_invalid_type_to_single_bytes(void);

void test_cvt_json_number_to_single_bytes(void);
void test_cvt_json_object_to_single_bytes(void);
void test_cvt_json_array_to_single_bytes(void);
void test_cvt_json_bool_to_single_bytes(void);

/******************************************************************************/
/*                              Global Variable                               */
/******************************************************************************/

const char bytes_field_name[]          = "f_bytes";
const char repeated_bytes_field_name[] = "f_repeated_bytes";

CU_TestInfo test_byte_conversion[] = {
    {"Convert JSON hex string to Protobuf bytes field",          test_cvt_json_hex_string_to_single_bytes     },
    {"Convert JSON base64 string to Protobuf bytes field",       test_cvt_json_base64_string_to_single_bytes  },
    {"Convert JSON filepath string to Protobuf bytes field",     test_cvt_json_filepath_string_to_single_bytes},
    {"Convert JSON base64 string to Protobuf oneof bytes field", test_cvt_json_base64_string_to_oneof_bytes   },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_invalid_byte_conversion[] = {
    {"Reject conversion of invalid JSON hex string to Protobuf bytes field",      test_cvt_invalid_hex_string_to_single_bytes     },
    {"Reject conversion of invalid JSON base64 string to Protobuf bytes field",   test_cvt_invalid_base64_string_to_single_bytes  },
    {"Reject conversion of invalid JSON filepath string to Protobuf bytes field", test_cvt_invalid_filepath_string_to_single_bytes},
    {"Reject conversion of invalid file type to Protobuf oneof bytes field",      test_cvt_invalid_type_to_single_bytes           },
    CU_TEST_INFO_NULL,
};

CU_TestInfo test_unacceptable_type_conversion[] = {
    {"Reject conversion of JSON number string to Protobuf bytes field",     test_cvt_json_number_to_single_bytes},
    {"Reject conversion of JSON object string to Protobuf bytes field",     test_cvt_json_object_to_single_bytes},
    {"Reject conversion of JSON array string to Protobuf bytes field",      test_cvt_json_array_to_single_bytes },
    {"Reject conversion of JSON bool string to Protobuf oneof bytes field", test_cvt_json_bool_to_single_bytes  },
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] = {
    {"Convert JSON to Protobuf byte field",                                init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion, test_byte_conversion},
    {"Reject conversion of invalid input to Protobuf byte field",          init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_invalid_byte_conversion                                                                                                                                                                },
    {"Reject conversion of unacceptable JSON type to Protobuf byte field", init_sutie_name, cleanup_sutie_name, setup_successful_conversion, teardown_successful_conversion,
     test_unacceptable_type_conversion                                                                                                                                                           },
    CU_SUITE_INFO_NULL,
};

/******************************************************************************/
/*                                 Test Cases                                 */
/******************************************************************************/

void
test_cvt_json_hex_string_to_single_bytes(void) {
    cJSON_AddStringToObject(root, bytes_field_name, "01 10 11 04");
    uint8_t      expected_bytes[] = {0x01, 0x10, 0x11, 0x04};
    const size_t expected_size    = sizeof(expected_bytes) / sizeof(expected_bytes[0]);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, J2P_FILE_HEX_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_SUCCESS);
    CU_ASSERT_EQUAL(msg->f_bytes.len, expected_size);
    CU_ASSERT_EQUAL(memcmp(msg->f_bytes.data, expected_bytes, expected_size), 0);
}

void
test_cvt_json_base64_string_to_single_bytes(void) {
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
test_cvt_json_filepath_string_to_single_bytes(void) {
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
test_cvt_json_base64_string_to_oneof_bytes(void) {
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

void
test_cvt_invalid_hex_string_to_single_bytes(void) {
    cJSON_AddStringToObject(root, bytes_field_name, "01 10 11 3 4");

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, J2P_FILE_HEX_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_HEX_STRING);
    CU_ASSERT_EQUAL(msg->f_bytes.len, 0);
    CU_ASSERT_EQUAL(msg->f_bytes.data, NULL);
}

void
test_cvt_invalid_base64_string_to_single_bytes(void) {
    uint8_t      expected_bytes[] = {0x01, 0x10, 0x11, 0x04};
    const size_t expected_size    = sizeof(expected_bytes) / sizeof(expected_bytes[0]);

    size_t out_len    = 0;
    char*  base64_str = b64_encode_with_alloc(expected_bytes, expected_size, &out_len);

    cJSON_AddStringToObject(root, bytes_field_name, base64_str + 1);
    free(base64_str);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, J2P_FILE_BASE64_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_BASE64_STRING);
    CU_ASSERT_EQUAL(msg->f_bytes.len, 0);
    CU_ASSERT_EQUAL(msg->f_bytes.data, NULL);
}

void
test_cvt_invalid_filepath_string_to_single_bytes(void) {
    cJSON_AddStringToObject(root, bytes_field_name, "test_does_not_exit.bin");

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, J2P_FILE_PATH_STR);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_FILE_PATH);
    CU_ASSERT_EQUAL(msg->f_bytes.len, 0);
    CU_ASSERT_EQUAL(msg->f_bytes.data, NULL);
}

void
test_cvt_invalid_type_to_single_bytes(void) {
    uint8_t      expected_bytes[] = {0x01, 0x10, 0x11, 0x04};
    const size_t expected_size    = sizeof(expected_bytes) / sizeof(expected_bytes[0]);

    size_t out_len    = 0;
    char*  base64_str = b64_encode_with_alloc(expected_bytes, expected_size, &out_len);

    cJSON_AddStringToObject(root, bytes_field_name, base64_str);
    free(base64_str);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, 15);
    CU_ASSERT_EQUAL(e, J2P_EXPT_INVALID_FILE_MODE);
    CU_ASSERT_EQUAL(msg->f_bytes.len, 0);
    CU_ASSERT_EQUAL(msg->f_bytes.data, NULL);
}

void
test_cvt_json_number_to_single_bytes(void) {
    cJSON_AddNumberToObject(root, bytes_field_name, 1);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, 15);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_bytes.len, 0);
    CU_ASSERT_EQUAL(msg->f_bytes.data, NULL);
}

void
test_cvt_json_object_to_single_bytes(void) {
    cJSON_AddObjectToObject(root, bytes_field_name);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, 15);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_bytes.len, 0);
    CU_ASSERT_EQUAL(msg->f_bytes.data, NULL);
}

void
test_cvt_json_array_to_single_bytes(void) {
    cJSON_AddArrayToObject(root, bytes_field_name);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, 15);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_bytes.len, 0);
    CU_ASSERT_EQUAL(msg->f_bytes.data, NULL);
}

void
test_cvt_json_bool_to_single_bytes(void) {
    cJSON_AddTrueToObject(root, bytes_field_name);

    j2p_expt_t e = cvt_json_2_pb_bytes(root, cJSON_GetObjectItem(root, bytes_field_name), (ProtobufCMessage*)msg, bytes_field_name, 15);
    CU_ASSERT_EQUAL(e, J2P_EXPT_UNACCEPTABLE_JSON_TYPE);
    CU_ASSERT_EQUAL(msg->f_bytes.len, 0);
    CU_ASSERT_EQUAL(msg->f_bytes.data, NULL);
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
