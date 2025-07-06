/**
 * @file json2pb.h
 * @author 陆光中 (luke_guangzhong@petalmail.com)
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

    J2P_EXPT_VALUE_OVERFLOW,
    J2P_EXPT_UNACCEPTABLE_JSON_TYPE,
    J2P_EXPT_INVALID_NUMBER_STRING,
    J2P_EXPT_NOT_EXACT,
    J2P_EXPT_INVALID_ENUM_VALUE,
    J2P_EXPT_INVALID_HEX_STRING,
    J2P_EXPT_INVALID_BASE64_STRING,
    J2P_EXPT_INVALID_FILE_PATH,

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
    J2P_EXPT_INVALID_FILE_MODE,
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

typedef int (*string_enum_convertor)(const ProtobufCEnumDescriptor* const enum_desc, const char* const str);

typedef j2p_expt_t (*obj_msg_convertor)(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg);

typedef int (*compand_convertor)(void* p1, void* p2);

typedef j2p_expt_t (*single_field_cvt_func)(const cJSON* const item, void* const field);

typedef union json2pb_additional_convertor {
    j2p_file_t            file_type;
    string_bool_convertor bool_cvt;
    string_enum_convertor enum_cvt;
    obj_msg_convertor     msg_cvt;
} j2p_add_cvt;

typedef j2p_expt_t (*single_field_cvt_func_v2)(const cJSON* const item, void* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt);

j2p_expt_t cvt_json_2_pb_field(const cJSON*                root,
                               const cJSON*                item,
                               ProtobufCMessage* const     msg,
                               const char* const           field_name,
                               const string_bool_convertor bool_cvt,
                               const string_enum_convertor enum_cvt,
                               const j2p_file_t            file_type,
                               const obj_msg_convertor     msg_cvt);

j2p_expt_t cvt_json_2_pb_field_v2(const cJSON* root, const cJSON* item, ProtobufCMessage* const msg, const char* const field_name, const j2p_add_cvt add_cvt);

j2p_expt_t cvt_json_2_pb_number(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name);

j2p_expt_t cvt_json_2_pb_bool(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, string_bool_convertor str_bool_cvt_func);

j2p_expt_t cvt_json_2_pb_enum(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, string_enum_convertor str_enum_cvt_func);

j2p_expt_t cvt_json_2_pb_string(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name);

j2p_expt_t cvt_json_2_pb_bytes(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, const j2p_file_t file_type);

j2p_expt_t cvt_json_2_pb_message(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, obj_msg_convertor msg_cvt_func);