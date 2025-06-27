/**
 * @file json2pb.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <cjson/cJSON.h>
#include <protobuf-c/protobuf-c.h>
#include <stdbool.h>

typedef enum json2pb_exception_type {
    J2P_EXPT_SUCCESS = 0,

    J2P_EXPT_NULL_VALUE,
    J2P_EXPT_VALUE_OVERFLOW,
    J2P_EXPT_UNACCEPTABLE_JSON_TYPE,
    J2P_EXPT_INVALID_NUMBER_STRING,

    J2P_EXPT_EMPTY_ARRAY,
    J2P_EXPT_NO_VALID_FOUND,
    J2P_EXPT_PARTIAL_FAIL,
    J2P_EXPT_JSON_POINTER_NOT_FOUND,
    J2P_EXPT_JSON_GENERAL,

    J2P_EXPT_UNINITIALIZED,
    J2P_EXPT_FIELD_NOT_FOUND,
    J2P_EXPT_FIELD_IS_DEPRECATED,
    J2P_EXPT_ONEOF_ALREADY_SET,
    J2P_EXPT_PB_GENERAL,

    J2P_EXPT_INVALID_ARG,
    J2P_EXPT_INCORRECT_EXCEPTION_TYPE,
    J2P_EXPT_CODE_GENERAL,

    J2P_EXPT_MEM_ALLOC_FAILED,
    J2P_EXPT_OS_GENERAL,
} j2p_expt_t;

typedef enum json2pb_file_code {
    J2P_FILE_PATH_STR,
    J2P_FILE_BASE64_STR,
    J2P_FILE_HEX_STR,
} j2p_file_t;

typedef struct json2pb_exception_msg {
    const j2p_expt_t type;
    const char*      desc;
} j2p_expt_msg;

extern const j2p_expt_msg j2p_expt_msg_list[];

typedef bool (*string_bool_convertor)(const char* const str);

typedef int (*string_enum_convertor)(const char* const str);

j2p_expt_t cvt_json_2_pb_field(const cJSON*                root,
                               const cJSON*                item,
                               ProtobufCMessage* const     msg,
                               const char* const           field_name,
                               const string_bool_convertor bool_cvt,
                               const string_enum_convertor enum_cvt,
                               const j2p_file_t            file_type);

j2p_expt_t cvt_json_2_pb_number(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name);