/**
 * @file internal.h
 * @author 陆光中 (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <stdint.h>

#include "json2pb.h"

/* single convertors */

/* for v1 */

j2p_expt_t cvt_single_int32_t(const cJSON* const item, int32_t* const field);

j2p_expt_t cvt_single_int64_t(const cJSON* const item, int64_t* const field);

j2p_expt_t cvt_single_uint32_t(const cJSON* const item, uint32_t* const field);

j2p_expt_t cvt_single_uint64_t(const cJSON* const item, uint64_t* const field);

j2p_expt_t cvt_single_float(const cJSON* const item, float* const field);

j2p_expt_t cvt_single_double(const cJSON* const item, double* const field);

j2p_expt_t cvt_single_bool(const cJSON* const item, bool* const field, string_bool_convertor bool_cvt);

j2p_expt_t cvt_single_enum(const cJSON* const item, int* const field, string_enum_convertor str_enum_cvt, const ProtobufCEnumDescriptor* const enum_desc);

j2p_expt_t cvt_single_string(const cJSON* const item, char** const field);

j2p_expt_t cvt_single_bytes(const cJSON* const item, ProtobufCBinaryData* const field, j2p_file_t mode);

j2p_expt_t
cvt_single_message(const cJSON* const root, const cJSON* const item, ProtobufCMessage** const field, obj_msg_convertor obj_cvt, const ProtobufCMessageDescriptor* const msg_desc);

/* for v2 */

j2p_expt_t
cvt_single_int32_t_v2(const cJSON* const root, const cJSON* const item, int32_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t
cvt_single_int64_t_v2(const cJSON* const root, const cJSON* const item, int64_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t
cvt_single_uint32_t_v2(const cJSON* const root, const cJSON* const item, uint32_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t
cvt_single_uint64_t_v2(const cJSON* const root, const cJSON* const item, uint64_t* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t
cvt_single_float_v2(const cJSON* const root, const cJSON* const item, float* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t
cvt_single_double_v2(const cJSON* const root, const cJSON* const item, double* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t
cvt_single_bool_v2(const cJSON* const root, const cJSON* const item, bool* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t cvt_single_enum_v2(const cJSON* const root, const cJSON* const item, int* const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t
cvt_single_string_v2(const cJSON* const root, const cJSON* const item, char** const field, const ProtobufCFieldDescriptor* const field_desc, const j2p_add_cvt add_cvt_func);

j2p_expt_t cvt_single_bytes_v2(const cJSON* const                    root,
                               const cJSON* const                    item,
                               ProtobufCBinaryData* const            field,
                               const ProtobufCFieldDescriptor* const field_desc,
                               const j2p_add_cvt                     add_cvt_func);

j2p_expt_t cvt_single_message_v2(const cJSON* const                    root,
                                 const cJSON* const                    item,
                                 ProtobufCMessage** const              field,
                                 const ProtobufCFieldDescriptor* const field_desc,
                                 const j2p_add_cvt                     add_cvt_func);

/* default convertors */

bool default_string_bool_convertor(const char* const str);

int default_string_bool_convertor_v2(const ProtobufCFieldDescriptor* const field_desc, const char* const str);

int default_string_enum_convertor(const ProtobufCEnumDescriptor* const enum_desc, const char* const str);

j2p_expt_t default_obj_msg_convertor(const cJSON* const root, const cJSON* const item, ProtobufCMessage* const msg);

/* utils */

int int_range_lookup(unsigned n_ranges, const ProtobufCIntRange* ranges, int value);

const ProtobufCEnumValue* protobuf_c_enum_descriptor_get_value_by_name_case_insensitive(const ProtobufCEnumDescriptor* desc, const char* name);

j2p_expt_t util_cvt_hex_to_bytes(const char* const hex_str, uint8_t** const bytes, size_t* const bytes_len);

j2p_expt_t util_cvt_base64_to_bytes(const char* const base64_str, uint8_t** const bytes, size_t* const bytes_len);

j2p_expt_t util_cvt_file_to_bytes(const char* const file_path, uint8_t** const bytes, size_t* const bytes_len);
