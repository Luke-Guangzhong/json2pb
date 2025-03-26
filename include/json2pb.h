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

#include "cjson/cJSON.h"
#include <protobuf-c/protobuf-c.h>

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
    const char*      message;
} j2p_expt_msg;

extern const j2p_expt_msg j2p_expt_msg_list[];

typedef struct json2pb_exception {
    char*               where;
    const j2p_expt_msg* msg;
} j2p_expt;

/**
 * @brief Generate a json2pb exception.
 *
 * @param[in] root JSON root object.
 * @param[in] item Current item converting to protobuf message.
 * @param[in] type Exception type
 * @return j2p_expt* pointer to the generated exception.
 * @retval NULL if exception is not generated.
 */
j2p_expt* gen_json2pb_exception(const cJSON* root, const cJSON* item, const j2p_expt_t type);

#ifndef JSON2PB_THROW_EXCEPTION
#define JSON2PB_THROW_EXCEPTION(e) return gen_json2pb_exception(root, item, e)
#endif

/**
 * @brief Free a json2pb exception structure.
 *
 * @param e
 */
void free_json2pb_exception(j2p_expt* e);

#ifndef FREE_JSON2PB_EXCEPTION
#define FREE_JSON2PB_EXCEPTION(e)  \
    do {                           \
        free_json2pb_exception(e); \
        e = NULL;                  \
    } while (false)
#endif

typedef bool (*StringBoolCallback)(const char* const str);

j2p_expt* cvt_cjson_2_proto_c_field(const cJSON* restrict root, ProtobufCMessage* restrict msg, const cJSON* restrict item, const char* restrict field_name, StringBoolCallback string_bool_cb);