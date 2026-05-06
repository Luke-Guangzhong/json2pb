/**
 * @file easy_interface.c
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-06-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "json2pb.h"

j2p_expt_t
cvt_json_2_pb_number(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name)
{
    j2p_add_cvt add_cvt = {.file_type = J2P_FILE_BASE64_STR};
    return cvt_json_2_pb_field_v2(root, item, msg, field_name, add_cvt);
}

j2p_expt_t
cvt_json_2_pb_bool(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, string_bool_convertor str_bool_cvt_func)
{
    j2p_add_cvt add_cvt = {.bool_cvt = str_bool_cvt_func};
    return cvt_json_2_pb_field_v2(root, item, msg, field_name, add_cvt);
}

j2p_expt_t
cvt_json_2_pb_enum(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, string_enum_convertor str_enum_cvt_func)
{
    j2p_add_cvt add_cvt = {.enum_cvt = str_enum_cvt_func};
    return cvt_json_2_pb_field_v2(root, item, msg, field_name, add_cvt);
}

j2p_expt_t
cvt_json_2_pb_string(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name)
{
    j2p_add_cvt add_cvt = {.file_type = J2P_FILE_BASE64_STR};
    return cvt_json_2_pb_field_v2(root, item, msg, field_name, add_cvt);
}

j2p_expt_t
cvt_json_2_pb_bytes(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, const j2p_file_t file_type)
{
    j2p_add_cvt add_cvt = {.file_type = file_type};
    return cvt_json_2_pb_field_v2(root, item, msg, field_name, add_cvt);
}

j2p_expt_t
cvt_json_2_pb_message(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, obj_msg_convertor msg_cvt_func)
{
    j2p_add_cvt add_cvt = {.msg_cvt = msg_cvt_func};
    return cvt_json_2_pb_field_v2(root, item, msg, field_name, add_cvt);
}