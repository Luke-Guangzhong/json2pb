/**
 * @file cjson2pb_easy_interface.c
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @date 2024-08-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "cjson2pb-c.h"

uint32_t
cvt_cjson_2_pb_number(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name)
{
    if (NULL == root || NULL == msg || NULL == field_name) {
        return E_INVALID_ARG;
    }

    return cvt_cjson_2_proto_c_field(root, msg, field_name, NULL, NULL, NULL, BYTE_MODE_FILE_PATH);
}

uint32_t
cvt_cjson_2_pb_string(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name)
{
    if (NULL == root || NULL == msg || NULL == field_name) {
        return E_INVALID_ARG;
    }

    return cvt_cjson_2_proto_c_field(root, msg, field_name, NULL, NULL, NULL, BYTE_MODE_FILE_PATH);
}

uint32_t
cvt_cjson_2_pb_message(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name, MsgConvertorCallback const msg_convertor)
{
    if (NULL == root || NULL == msg || NULL == field_name) {
        return E_INVALID_ARG;
    }

    return cvt_cjson_2_proto_c_field(root, msg, field_name, NULL, NULL, msg_convertor, BYTE_MODE_FILE_PATH);
}

uint32_t
cvt_cjson_2_pb_enum(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name, StringEnumCallback const string_enum)
{
    if (NULL == root || NULL == msg || NULL == field_name) {
        return E_INVALID_ARG;
    }

    return cvt_cjson_2_proto_c_field(root, msg, field_name, string_enum, NULL, NULL, BYTE_MODE_FILE_PATH);
}

uint32_t
cvt_cjson_2_pb_byte(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name, const ByteMode mode)
{
    if (NULL == root || NULL == msg || NULL == field_name) {
        return E_INVALID_ARG;
    }

    return cvt_cjson_2_proto_c_field(root, msg, field_name, NULL, NULL, NULL, mode);
}

uint32_t
cvt_cjson_2_pb_bool(const cJSON* const restrict root, ProtobufCMessage* const restrict msg, const char* const restrict field_name, StringBoolCallback const string_bool)
{
    if (NULL == root || NULL == msg || NULL == field_name) {
        return E_INVALID_ARG;
    }

    return cvt_cjson_2_proto_c_field(root, msg, field_name, NULL, NULL, NULL, BYTE_MODE_FILE_PATH);
}