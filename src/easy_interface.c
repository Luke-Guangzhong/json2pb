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
    return cvt_json_2_pb_field(root, item, msg, field_name, NULL, NULL, J2P_FILE_PATH_STR, NULL);
}

j2p_expt_t
cvt_json_2_pb_bool(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, string_bool_convertor str_bool_cvt_func)
{
    return cvt_json_2_pb_field(root, item, msg, field_name, str_bool_cvt_func, NULL, J2P_FILE_PATH_STR, NULL);
}

j2p_expt_t
cvt_json_2_pb_enum(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, string_enum_convertor str_enum_cvt_func)
{
    return cvt_json_2_pb_field(root, item, msg, field_name, NULL, str_enum_cvt_func, J2P_FILE_PATH_STR, NULL);
}

j2p_expt_t
cvt_json_2_pb_string(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name)
{
    return cvt_json_2_pb_field(root, item, msg, field_name, NULL, NULL, J2P_FILE_PATH_STR, NULL);
}

j2p_expt_t
cvt_json_2_pb_bytes(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, const j2p_file_t file_type)
{
    return cvt_json_2_pb_field(root, item, msg, field_name, NULL, NULL, file_type, NULL);
}

j2p_expt_t
cvt_json_2_pb_message(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg, const char* const field_name, obj_msg_convertor msg_cvt_func)
{
    return cvt_json_2_pb_field(root, item, msg, field_name, NULL, NULL, J2P_FILE_PATH_STR, msg_cvt_func);
}