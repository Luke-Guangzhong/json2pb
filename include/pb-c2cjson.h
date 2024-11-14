/**
 * @file pb-c2cjson.h
 * @author luguangzhong (luke_guangzhong@petalmail.com)
 * @brief
 * @version 0.1
 * @date 2024-11-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "global.h"

#ifdef __GNUC__
#define ALL_NONNULL  __attribute__((__nonnull__))
#define NONNULL(...) __attribute__((__nonnull__(__VA_ARGS__)))
#else
#define ALL_NONNULL
#define NONNULL(...)
#endif

cJSON* gen_cjson_via_pb_number(const ProtobufCMessage* const pb_msg, const ProtobufCFieldDescriptor* const field_desc, const bool show_null_value) NONNULL(1, 2);
cJSON* gen_cjson_via_pb_bool(const ProtobufCMessage* const pb_msg, const ProtobufCFieldDescriptor* const field_desc, const bool show_null_value) NONNULL(1, 2);
cJSON* gen_cjson_via_pb_enum(const ProtobufCMessage* const pb_msg, const ProtobufCFieldDescriptor* const field_desc, const bool show_null_value, const bool enum_string) NONNULL(1, 2);
cJSON* gen_cjson_via_pb_string(const ProtobufCMessage* const pb_msg, const ProtobufCFieldDescriptor* const field_desc, const bool show_null_value) NONNULL(1, 2);
cJSON* gen_cjson_via_pb_bytes(const ProtobufCMessage* const pb_msg, const ProtobufCFieldDescriptor* const field_desc, const bool show_null_value, const ByteMode byte_mode) NONNULL(1, 2);
cJSON* gen_cjson_via_pb_msg(const ProtobufCMessage* const pb_msg, const ProtobufCFieldDescriptor* const field_desc, const bool show_null_value, const bool enum_string, const ByteMode byte_mode) NONNULL(1, 2);

cJSON* gen_cjson_via_pb_field(const ProtobufCMessage* pb_msg, const char* field_name, const bool show_null_value, const bool enum_string, ByteMode byte_mode);

cJSON* gen_cjson_via_pb_msg_single(const ProtobufCMessage* const restrict pb_msg, const bool show_null_value, const bool enum_string, const ByteMode byte_mode);
cJSON* gen_cjson_via_pb_enum_single(const int32_t enum_value, const ProtobufCEnumDescriptor* const restrict enum_desc, const bool enum_string);
cJSON* gen_cjson_via_pb_bytes_single(const ProtobufCBinaryData bin_data, const ProtobufCFieldDescriptor* field_desc, const bool show_null_value, const ByteMode byte_mode);

#undef ALL_NONNULL
#undef NONNULL