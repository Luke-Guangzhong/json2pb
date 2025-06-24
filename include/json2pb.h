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
    JSON2PB_SUCCESS = 0,

    JSON2PB_NULL_VALUE,
    JSON2PB_VALUE_OVERFLOW,
    JSON2PB_UNACCEPTABLE_JSON_TYPE,
    JSON2PB_INVALID_NUMBER_STRING,
    JSON2PB_EMPTY_ARRAY,
    JSON2PB_JSON_GENERAL,

    JSON2PB_UNINITIALIZED,
    JSON2PB_FIELD_NOT_FOUND,
    JSON2PB_FIELD_IS_DEPRECATED,
    JSON2PB_ONEOF_ALREADY_SET,
    JSON2PB_PB_GENERAL,

    JSON2PB_INVALID_ARG,
    JSON2PB_INCORRECT_EXCEPTION_TYPE,
    JSON2PB_CODE_GENERAL,

    JSON2PB_MEM_ALLOC_FAILED,
    JSON2PB_OS_GENERAL,
} j2p_expt_t;

typedef struct json2pb_exception_msg {
    const j2p_expt_t type;
    const char*      desc;
} j2p_expt_msg;

extern const j2p_expt_msg j2p_expt_msg_list[];

typedef bool (*string_bool_convertor)(const char* const str);

typedef int (*string_enum_convertor)(const char* const str);

j2p_expt_t cvt_cjson_2_proto_c_field(const cJSON* restrict root, ProtobufCMessage* const msg, const cJSON* restrict item, const char* const field_name, const string_bool_convertor bool_cvt, const string_enum_convertor enum_cvt);
